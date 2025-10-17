// --- PIN DEFINITIONS ---
// Define the analog input pins for voltage and current sensing
#define vin_pin A1                 // Analog input for Sensed Input Voltage (Vin)
#define output_voltage_sense A2    // Analog input for Sensed Output Voltage (Vout)
#define input_voltage_sense A0     // Analog input for Sensed Output Current (Iout)

// Define the digital PWM pins for the Buck and Boost MOSFET drivers
#define boost_pin 5                // PWM pin for the Boost switch (MOSFET)
#define buck_pin 6                 // PWM pin for the Buck switch (MOSFET)

// --- GLOBAL VARIABLES (RAW READINGS AND THRESHOLDS) ---
// Variables to hold the raw (0-1023) analog readings before averaging/scaling
int raw_vin = 0, raw_vout = 0, raw_iout = 0;

// User-defined voltage and current thresholds (Target and Limits)
float Vout_max = 13.0, Iout_max = 1.0;  // Target Output Voltage and Maximum Allowed Current
float Vout_min = 11.1, Iout_min = 0.1;  // Minimum Vout to exit soft-start, Minimum Iout for cutoff
float Vin_thresold = 10.5;              // Minimum Vin threshold for auto-cutoff

// Variables to hold the final calculated voltage and current values (Volts/Amps)
float Iout_sense;
float Vout_sense;
float Vin_sense;

// Control variable: PWM duty cycle (0-255)
uint8_t duty_cycle = 25;

// Status variables
String mode = "";         // Holds the current converter mode ("Buck mode" or "Boost mode")
bool startup = true;      // Flag to control the Soft Start routine
unsigned int count = 0;   // Counter for slow serial printing

// --- SETUP: Runs once when the sketch starts ---
void setup() {
    // Initialize serial communication for debugging and user input
    Serial.begin(115200);

    // Fast PWM Configuration for Timer 0 (pins 5 and 6 use Timer 0 on Uno)
    // TCCR0B register manipulation: sets the PWM prescaler to 1 for higher frequency.
    TCCR0B = TCCR0B & 0b11111000 | 0x01;

    // Initialize the converter: Buck switch ON, Boost switch OFF (Default safe state/Buck)
    analogWrite(buck_pin, 255); // Buck: 255 means 0% duty cycle (OFF for high-side switch)
    analogWrite(boost_pin, 0);  // Boost: 0 means 0% duty cycle (OFF)
}

// --- MAIN LOOP: Runs continuously ---
void loop() {
    // 1. SERIAL INPUT: Allows changing Vout_max dynamically
    if (Serial.available()) {
        String data = Serial.readString();
        // User sends an integer, which is scaled (e.g., 130 becomes 13.0V)
        Vout_max = data.toInt();
        Vout_max = Vout_max / 10.0;
        Serial.print("Vout_max= ");
        Serial.println(Vout_max);
    }

    // 2. SENSOR READING AND AVERAGING (Noise Reduction)
    // Reset raw accumulators
    raw_iout = 0;
    raw_vin = 0;
    raw_vout = 0;

    for (int i = 0; i < 10; i++) {
        // Current Sense: Reading assumes a sensor centered at 512 (2.5V) for zero current
        raw_iout += analogRead(input_voltage_sense) - 513;
        raw_vin += analogRead(vin_pin);
        raw_vout += analogRead(output_voltage_sense);
    }

    // Calculate the average readings
    raw_iout /= 10;
    raw_vout /= 10;
    raw_vin /= 10;

    // 3. SCALING (Convert raw ADC values to physical units)
    // Scaling factors are determined by external voltage/current sensor hardware
    Iout_sense = float(raw_iout) * 0.0586; // Example: 0.0586 Amps per ADC unit
    Vout_sense = float(raw_vout) * 0.046;  // Example: 0.046 Volts per ADC unit
    Vin_sense = float(raw_vin) * 0.046;    // Example: 0.046 Volts per ADC unit

    // 4. STATUS PRINTING (Executed every 101 loops)
    if (count > 100) {
        Serial.print("Vin= "); Serial.println(Vin_sense);
        Serial.print("Vout= "); Serial.println(Vout_sense);
        Serial.print("Iout= "); Serial.println(Iout_sense);
        Serial.print("Duty cycle="); Serial.println(duty_cycle);
        Serial.print("Converter MODE : "); Serial.println(mode);
        count = 0;
    }

    // 5. CONTROL LOGIC
    if (!startup) {
        // Normal Operation: Run PID-like control and safety cutoff
        regulate(Iout_sense, Vin_sense, Vout_sense);
        auto_cutoff(Iout_sense, Vin_sense, Vout_sense);
    } else {
        // Startup Sequence
        soft_start();
    }

    // Wait and increment loop counter
    delay(600); // Main loop delay (controls sample rate and response time)
    count++;
}

// --- REGULATION FUNCTION (Main Control Logic) ---
void regulate(float Iout, float Vin, float Vout) {
    // --- MODE SELECTION: BUCK MODE ---
    if (Vout_max < Vin) {
        mode = "Buck mode";
        analogWrite(boost_pin, 0); // Ensure Boost switch is off

        // REGULATION UP (Vout is low, Iout is normal)
        if ((Iout < Iout_max && Iout > Iout_min) && (Vout < Vout_max)) {
            if (duty_cycle < 250) {
                duty_cycle += 2; // Increase duty cycle (P-term: proportional change)
            }
            // PWM is inverted for Buck (analogWrite(pin, 255 - duty_cycle) for high-side switch)
            analogWrite(buck_pin, 255 - duty_cycle);
        }
        // REGULATION DOWN (Vout is high OR Iout is too high)
        else if ((Iout > Iout_max) || (Vout > Vout_max)) {
            if (duty_cycle > 2) {
                duty_cycle -= 2; // Decrease duty cycle
            }
            analogWrite(buck_pin, 255 - duty_cycle);
        }
    }

    // --- MODE SELECTION: BOOST MODE ---
    else if (Vout_max > Vin) {
        mode = "Boost mode";
        analogWrite(buck_pin, 0); // Ensure Buck switch is off

        // REGULATION UP (Vout is low, Iout is normal)
        if ((Iout < Iout_max) && (Vout < Vout_max)) {
            if (duty_cycle < 220) {
                duty_cycle += 2; // Increase duty cycle
            }
            // PWM is standard for Boost (analogWrite(pin, duty_cycle))
            analogWrite(boost_pin, duty_cycle);
        }
        // REGULATION DOWN (Iout is too high OR Vout is too high)
        else if ((Iout > Iout_max) || (Vout > Vout_max)) {
            if (duty_cycle > 4) {
                duty_cycle -= 2; // Decrease duty cycle
            }
            analogWrite(boost_pin, duty_cycle);
        }
    }
}

// --- AUTO CUTOFF FUNCTION (Safety Shutdown) ---
void auto_cutoff(float Iout, float Vin, float Vout) {
    // Condition 1: Charging Completed (Vout reached target AND current dropped)
    // OR
    // Condition 2: Input voltage too low
    if ((Vout >= Vout_max && Iout < Iout_min) || (Vin < Vin_thresold)) {
        // Shut down both PWM outputs
        analogWrite(boost_pin, 0);
        analogWrite(buck_pin, 255);
        Serial.println("Charging Completed.");
        delay(64000); // Long delay to pause operation after completion/shutdown
    }
}

// --- SOFT START FUNCTION (Controlled Power-Up) ---
void soft_start() {
    // Keep running soft start until Vout is above the minimum safe level
    if (Vout_sense <= Vout_min) {
        // Run regulation (this will incrementally increase the duty cycle)
        regulate(Iout_sense, Vin_sense, Vout_sense);

        // Debug printing during soft start
        Serial.print("Vin= "); Serial.println(Vin_sense);
        Serial.print("Vout= "); Serial.println(Vout_sense);
        Serial.print("Iout= "); Serial.println(Iout_sense);
        Serial.print("Duty cycle="); Serial.println(duty_cycle);
        Serial.print("Converter MODE : "); Serial.println(mode);
        Serial.println("Soft Start Activated");
        
        delay(64000); // Long delay (use a smaller value like 100ms for testing!)
    } else {
        // Once Vout is stable, exit soft start
        startup = false;
    }
}
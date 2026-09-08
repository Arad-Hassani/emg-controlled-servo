#include <Servo.h>

// ------------------------------------------------------------
// EMG-Controlled Servo
//
// Reads an analog electromyography (EMG) signal and actuates
// a servo motor when muscle activity exceeds a set threshold.
//
// The detected EMG amplitude is mapped to a servo angle,
// allowing stronger muscle activation to produce a larger
// servo movement.
// ------------------------------------------------------------


// -------------------------
// Pin Configuration
// -------------------------

const int EMG_PIN   = A0;
const int SERVO_PIN = 2;


// -------------------------
// Signal Parameters
// -------------------------

// Raw ADC threshold used to identify significant muscle activity.
// Arduino ADC range: 0–1023.
const int EMG_THRESHOLD = 341;


// -------------------------
// Servo Parameters
// -------------------------

const int SERVO_MIN_ANGLE = 0;
const int SERVO_MAX_ANGLE = 180;

const int SAMPLE_DELAY_MS = 50;


// -------------------------
// Servo Object
// -------------------------

Servo servoMotor;


void setup()
{
    Serial.begin(115200);

    servoMotor.attach(SERVO_PIN);

    // Start servo in its resting position.
    servoMotor.write(SERVO_MIN_ANGLE);
}


void loop()
{
    // Read the amplified EMG signal from the analog input.
    int emgValue = analogRead(EMG_PIN);


    // --------------------------------------------------------
    // Serial output
    //
    // These values can be viewed using the Arduino Serial
    // Plotter to compare muscle activity with the activation
    // threshold.
    // --------------------------------------------------------

    Serial.print("EMG:");
    Serial.print(emgValue);

    Serial.print(",Threshold:");
    Serial.println(EMG_THRESHOLD);


    // --------------------------------------------------------
    // Servo Control
    // --------------------------------------------------------

    if (emgValue > EMG_THRESHOLD)
    {
        // Convert measured muscle activity into a servo angle.
        int servoAngle = map(
            emgValue,
            EMG_THRESHOLD,
            1023,
            SERVO_MIN_ANGLE,
            SERVO_MAX_ANGLE
        );

        // Ensure the result remains within valid servo limits.
        servoAngle = constrain(
            servoAngle,
            SERVO_MIN_ANGLE,
            SERVO_MAX_ANGLE
        );

        servoMotor.write(servoAngle);
    }
    else
    {
        // Return to resting position when muscle activity
        // falls below the activation threshold.
        servoMotor.write(SERVO_MIN_ANGLE);
    }


    delay(SAMPLE_DELAY_MS);
}

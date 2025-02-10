#define TOTAL_PINS 16
#define IC_COUNT 2  // Expanded for more ICs

struct GateTest {
    int numTests; // Number of test cases for this gate
    const char* testPatterns[10]; // Increased capacity for test cases
};

struct ICTest {
    const char* name;
    int numElements; // Number of gates or flip-flops
    const char* type; // Type of IC: "Gate", "Flip-Flop", etc.
    GateTest gateTests[4]; // Separate test cases for each gate
};

ICTest icDatabase[IC_COUNT] = {
    {   // 7400 - Quad 2-input NAND gate
        "7400",
        4, // Number of NAND gates
        "Gate",
        {{{4, {"LLHXXXXXXXXXXXXXX", "LHHXXXXXXXXXXXXXX", "HLHXXXXXXXXXXXXXX", "HHLXXXXXXXXXXXXXX"}},
          {4, {"LLHXXXXXXXXXXXXXX", "LHHXXXXXXXXXXXXXX", "HLHXXXXXXXXXXXXXX", "HHLXXXXXXXXXXXXXX"}},
          {4, {"LLHXXXXXXXXXXXXXX", "LHHXXXXXXXXXXXXXX", "HLHXXXXXXXXXXXXXX", "HHLXXXXXXXXXXXXXX"}},
          {4, {"LLHXXXXXXXXXXXXXX", "LHHXXXXXXXXXXXXXX", "HLHXXXXXXXXXXXXXX", "HHLXXXXXXXXXXXXXX"}}}}
    },
    {   // 7402 - Quad 2-input NOR gate
        "7402",
        4, // Number of NOR gates
        "Gate",
        {{{4, {"LLHXXXXXXXXXXXXXX", "LHHXXXXXXXXXXXXXX", "HLHXXXXXXXXXXXXXX", "HHLXXXXXXXXXXXXXX"}},
          {4, {"LLHXXXXXXXXXXXXXX", "LHHXXXXXXXXXXXXXX", "HLHXXXXXXXXXXXXXX", "HHLXXXXXXXXXXXXXX"}},
          {4, {"LLHXXXXXXXXXXXXXX", "LHHXXXXXXXXXXXXXX", "HLHXXXXXXXXXXXXXX", "HHLXXXXXXXXXXXXXX"}},
          {4, {"LLHXXXXXXXXXXXXXX", "LHHXXXXXXXXXXXXXX", "HLHXXXXXXXXXXXXXX", "HHLXXXXXXXXXXXXXX"}}}}
    }
};

void setup() {
    Serial.begin(9600);
    Serial.println("Enter IC name (e.g., 7400, 7402): ");
}

void loop() {
    if (Serial.available() > 0) {
        String inputIC = Serial.readStringUntil('\n');
        inputIC.trim();
        int icIndex = findIC(inputIC);
        if (icIndex != -1) {
            testIC(icIndex);
        } else {
            Serial.println("IC not found in database.");
        }
    }
}

int findIC(String icName) {
    for (int i = 0; i < IC_COUNT; i++) {
        if (String(icDatabase[i].name).equals(icName)) {
            return i;
        }
    }
    return -1;
}

void testIC(int icIndex) {
    ICTest& ic = icDatabase[icIndex];
    Serial.println("Testing IC: " + String(ic.name));
    Serial.print("Type: "); Serial.println(ic.type);
    Serial.print("Number of Elements: "); Serial.println(ic.numElements);
    
    for (int testIndex = 0; testIndex < ic.numElements; testIndex++) {
        Serial.print("Testing "); Serial.print(ic.type); Serial.print(" "); Serial.println(testIndex + 1);
        
        for (int testCase = 0; testCase < ic.gateTests[testIndex].numTests; testCase++) {
            const char* pattern = ic.gateTests[testIndex].testPatterns[testCase];
            if (strlen(pattern) == 0) break; // Skip if no test pattern
            
            for (int i = 0; i < TOTAL_PINS; i++) {
                char pinState = pattern[i];
                if (pinState == 'L' || pinState == 'H') {
                    pinMode(i, OUTPUT);
                    digitalWrite(i, (pinState == 'L') ? LOW : HIGH);
                } else if (pinState == 'X' || pinState == '0' || pinState == '1') {
                    pinMode(i, INPUT);
                }
            }
            
            delay(100); // Allow signals to stabilize
            
            bool gateWorking = true;
            for (int i = 0; i < TOTAL_PINS; i++) {
                char pinState = pattern[i];
                if (pinState == '0' || pinState == '1') {
                    int actualState = digitalRead(i);
                    if ((pinState == '0' && actualState != LOW) || (pinState == '1' && actualState != HIGH)) {
                        Serial.print("Error: ");
                        Serial.print(ic.type);
                        Serial.print(" ");
                        Serial.print(testIndex + 1);
                        Serial.println(" not working correctly.");
                        gateWorking = false;
                    }
                }
            }
            if (gateWorking) {
                Serial.print(ic.type);
                Serial.print(" ");
                Serial.print(testIndex + 1);
                Serial.println(" is working correctly.");
            }
            Serial.println("Test Applied");
        }
    }
}

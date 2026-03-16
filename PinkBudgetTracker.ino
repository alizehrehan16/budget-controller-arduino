const int expenseButton = 2;
const int incomeButton  = 3;

const int redLED    = 12;
const int yellowLED = 13;
const int greenLED  = 11;

int balance = 50;   // starting amount

// --- button edge-detect (so 1 press = 1 change) ---
bool lastExpense = HIGH;
bool lastIncome  = HIGH;

unsigned long lastPressTime = 0;
const unsigned long debounceMs = 160;  

// --- thresholds  ---
const int GREEN_MIN  = 70;   // >= 70 is green
const int YELLOW_MIN = 30;   // 30..69 is yellow
// < 30 is red

void showStatus() {
  // LED Logic
  if (balance >= GREEN_MIN) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    Serial.println("🌷 Status: Princess Safe Zone (GREEN) - amazing job princess");
  }
  else if (balance >= YELLOW_MIN) {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
    Serial.println("🎀 Status: Soft Limit Mode (YELLOW) — slow down bestie");
  }
  else {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);
    Serial.println("🚨 Status: No-Buy Emergency (RED) — step away from Sephora 😭");
  }
}

void startupSparkle() {
  // cute boot animation
  for (int i = 0; i < 2; i++) {
    digitalWrite(greenLED, HIGH); delay(120); digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH); delay(120); digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH); delay(120); digitalWrite(redLED, LOW);
  }
}

void setup() {
  pinMode(expenseButton, INPUT_PULLUP);
  pinMode(incomeButton, INPUT_PULLUP);

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  Serial.begin(9600);
  delay(200);

  Serial.println("✨ Pink Budget Tracker booting...");
  startupSparkle();

  Serial.print("💗 Starting Balance: $");
  Serial.println(balance);
  showStatus();
}

void loop() {
  bool expenseNow = digitalRead(expenseButton); // HIGH normally, LOW when pressed
  bool incomeNow  = digitalRead(incomeButton);

  unsigned long now = millis();

  // Expense = detect HIGH -> LOW transition
  if (lastExpense == HIGH && expenseNow == LOW && (now - lastPressTime) > debounceMs) {
    balance += 10;
    lastPressTime = now;

    Serial.println("🖤 Income Received (+$10)");
    Serial.print("Balance: $");
    Serial.println(balance);
    showStatus();
    Serial.println("---------------------------");
  }

  // Income = detect HIGH -> LOW transition
  if (lastIncome == HIGH && incomeNow == LOW && (now - lastPressTime) > debounceMs) {
    balance -= 10;
    lastPressTime = now;

    Serial.println("💗 Expense Logged (-$10)");
    Serial.print("Balance: $");
    Serial.println(balance);
    showStatus();
    Serial.println("---------------------------");
  }

  lastExpense = expenseNow;
  lastIncome  = incomeNow;
}

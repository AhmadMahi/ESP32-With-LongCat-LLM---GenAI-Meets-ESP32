#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Al-Imaad";
const char* password = "7760@_Ahmad";

const char* endpoint = "https://api.longcat.chat/openai/v1/chat/completions";
const char* apiKey = "";  // ⚠️ Replace with your actual API key

String userInput = "";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to WiFi!");
  Serial.println("Type your message below and press ENTER:");
}

void loop() {
  // Wait for serial input
  if (Serial.available() > 0) {
    userInput = Serial.readStringUntil('\n');
    userInput.trim();
    
    if (userInput.length() > 0) {
      Serial.println("\nSending to LongCat API...");
      
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(endpoint);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", String("Bearer ") + apiKey);

        // Combine user input with Ahmad’s introduction for better context
        String fullMessage = "My name is Ahmad. " + userInput;

        // Prepare JSON payload
        String payload = "{";
        payload += "\"model\": \"LongCat-Flash-Chat\",";
        payload += "\"messages\": [{\"role\": \"user\", \"content\": \"" + fullMessage + "\"}],";
        payload += "\"max_tokens\": 200,";
        payload += "\"temperature\": 0.8";
        payload += "}";

        int httpCode = http.POST(payload);

        if (httpCode > 0) {
          String response = http.getString();
          Serial.println("\nRaw Response:");
          Serial.println(response);

          // Parse JSON response
          DynamicJsonDocument doc(8192);
          DeserializationError error = deserializeJson(doc, response);
          if (!error) {
            if (doc["choices"][0]["message"]["content"]) {
              String message = doc["choices"][0]["message"]["content"];
              Serial.println("\n🤖 AI Response:");
              Serial.println(message);
            } else {
              Serial.println("⚠️ No content found in response.");
            }
          } else {
            Serial.println("⚠️ Failed to parse JSON.");
          }
        } else {
          Serial.printf("⚠️ HTTP POST failed, code: %d\n", httpCode);
        }

        http.end();
      } else {
        Serial.println("⚠️ WiFi not connected.");
      }

      Serial.println("\nType your next message:");
    }
  }
}

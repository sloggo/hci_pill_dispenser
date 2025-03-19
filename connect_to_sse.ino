#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// WiFi credentials
const char *ssid = "Ushen";
const char *password = "00000000";

// Your hosted server address
const char *serverAddress = "https://hci-pill-dispenser.onrender.com";

// Buffer for SSE data
const int BUFFER_SIZE = 1024;
char buffer[BUFFER_SIZE];
int bufferPos = 0;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("\nStarting WiFi connection attempt...");
    Serial.printf("Attempting to connect to: %s\n", ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected to WiFi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void processSSELine(String line)
{
    if (line.startsWith("data: "))
    {
        String jsonData = line.substring(6); // Remove "data: " prefix
        JSONVar eventData = JSON.parse(jsonData);

        if (JSON.typeof(eventData) == "undefined")
        {
            Serial.println("Failed to parse JSON!");
            return;
        }

        // Check if it's a ping message
        if (eventData.hasOwnProperty("type") && (const char *)eventData["type"] == "ping")
        {
            Serial.println("Received ping");
            return;
        }

        // Process dispense event
        if (eventData.hasOwnProperty("prescription_id"))
        {
            Serial.println("Received dispense event:");
            Serial.print("Prescription ID: ");
            Serial.println((int)eventData["prescription_id"]);
            Serial.print("Patient: ");
            Serial.println((const char *)eventData["patient_name"]);

            // Process medications
            JSONVar medications = eventData["medications"];
            for (int i = 0; i < medications.length(); i++)
            {
                Serial.print("Funnel ");
                Serial.print((int)medications[i]["funnel_id"]);
                Serial.print(": Dispense ");
                Serial.print((int)medications[i]["pills"]);
                Serial.print(" pills of ");
                Serial.println((const char *)medications[i]["medication"]);

                // Here you would add code to control your physical pill dispenser
                // For example: activateDispenser(medications[i]["funnel_id"], medications[i]["pills"]);
            }
        }
    }
}

void connectToSSE()
{
    HTTPClient http;

    String url = String(serverAddress) + "/events";
    Serial.print("Connecting to SSE endpoint: ");
    Serial.println(url);

    http.begin(url);
    http.addHeader("Accept", "text/event-stream");

    int httpCode = http.GET();

    if (httpCode > 0)
    {
        Serial.printf("HTTP Response code: %d\n", httpCode);

        WiFiClient *stream = http.getStreamPtr();

        while (http.connected())
        {
            while (stream->available())
            {
                char c = stream->read();

                if (c == '\n')
                {
                    buffer[bufferPos] = '\0';
                    String line = String(buffer);
                    if (line.length() > 0)
                    {
                        processSSELine(line);
                    }
                    bufferPos = 0;
                }
                else if (bufferPos < BUFFER_SIZE - 1)
                {
                    buffer[bufferPos++] = c;
                }
            }
        }
    }
    else
    {
        Serial.printf("Failed to connect to SSE endpoint. Error: %s\n",
                      http.errorToString(httpCode).c_str());
    }

    http.end();
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        connectToSSE();
        Serial.println("SSE connection closed. Reconnecting in 5 seconds...");
    }
    else
    {
        Serial.println("WiFi disconnected. Reconnecting...");
        WiFi.begin(ssid, password);
    }
    delay(5000);
}
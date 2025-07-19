/*
* Test Main for Horizontal Finish Line Detection
* This program initializes the Pixy2 camera, retrieves vectors, detects horizontal finish lines, and prints the results to the serial monitor.
*/

#include "Config.h"
#include "VectorsProcessing.h"
#include "geometry2D.h"
#include <Arduino.h>

// Pixy2 instance
Pixy2SPI_SS pixy;

// Function to initialize the Pixy2 camera
void setup() {
    // Initialize serial communication
    #if ENABLE_SERIAL_PRINT == 1
    Serial.begin(9600);
    while (!Serial) {
        delay(100); // Wait for the serial port to initialize
    }
    Serial.println("Serial initialized.");
    #endif

    // Initialize Pixy2
    int8_t pixyResult = pixy.init(SPI_SS_PIXY_1_PIN);
    

    // Set Pixy2 to "line" program
    pixyResult = pixy.changeProg("line");
    if (pixyResult != PIXY_RESULT_OK) {
        Serial.println("Failed to change Pixy2 program to 'line'!");
        while (true); // Halt execution if Pixy2 fails to change program
    }
    Serial.println("Pixy2 set to 'line' program.");
}

// Function to detect horizontal finish lines and print results
void loop() {
    // Clear Pixy2 vectors
    VectorsProcessing pixyVectorsProcessing;
    pixyVectorsProcessing.clear();

    // Retrieve vectors from Pixy2
    int8_t pixyResult = pixy.line.getAllFeatures(LINE_VECTOR, true);
    if (pixyResult >= 0) {
        // Store vectors in a vector container
        std::vector<Vector> vectors(pixy.line.numVectors);
        memcpy(vectors.data(), pixy.line.vectors, pixy.line.numVectors * sizeof(Vector));

        // Print retrieved vectors
        Serial.println("Retrieved vectors:");
        for (size_t i = 0; i < vectors.size(); i++) {
            Serial.print("Vector ");
            Serial.print(i);
            Serial.print(": Start(");
            Serial.print(vectors[i].m_x0);
            Serial.print(", ");
            Serial.print(vectors[i].m_y0);
            Serial.print(") End(");
            Serial.print(vectors[i].m_x1);
            Serial.print(", ");
            Serial.print(vectors[i].m_y1);
            Serial.println(")");
        }

        // Detect horizontal finish line
        FinishLine horizontalFinishLine = VectorsProcessing::findHorizontalFinishLine(vectors, 5.0f, 10.0f); // Example max error angle of 5 degrees and max distance of 10 units

        // Print horizontal finish line result
        if (VectorsProcessing::isVectorValid(horizontalFinishLine.leftSegment) && VectorsProcessing::isVectorValid(horizontalFinishLine.rightSegment)) {
            Serial.println("Horizontal Finish Line Detected:");
            Serial.print("Left Segment: Start(");
            Serial.print(horizontalFinishLine.leftSegment.m_x0);
            Serial.print(", ");
            Serial.print(horizontalFinishLine.leftSegment.m_y0);
            Serial.print(") End(");
            Serial.print(horizontalFinishLine.leftSegment.m_x1);
            Serial.print(", ");
            Serial.print(horizontalFinishLine.leftSegment.m_y1);
            Serial.println(")");

            Serial.print("Right Segment: Start(");
            Serial.print(horizontalFinishLine.rightSegment.m_x0);
            Serial.print(", ");
            Serial.print(horizontalFinishLine.rightSegment.m_y0);
            Serial.print(") End(");
            Serial.print(horizontalFinishLine.rightSegment.m_x1);
            Serial.print(", ");
            Serial.print(horizontalFinishLine.rightSegment.m_y1);
            Serial.println(")");
        } else {
            Serial.println("No horizontal finish line detected.");
        }
    } else {
        Serial.println("Failed to retrieve vectors from Pixy2.");
    }

    // Add a delay to avoid flooding the serial monitor
    delay(1000);
}

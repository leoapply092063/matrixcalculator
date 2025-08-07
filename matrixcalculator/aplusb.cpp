#include <sstream>
#include <string>
#include <iostream>
#include <emscripten.h>
#include <cstdlib>

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void processMatrix(int* matrixData, int size, int rows, int cols) {
        int mats = size / (cols * cols);

        int* resultMatrix = new int[rows * cols];
        for(int i = 0; i < rows * cols; i++){
            resultMatrix[i] = 0;
        }

        for (int  m = 0 ; m < mats ; m++){
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    int index = m * rows * cols + r * cols + c;
                    resultMatrix[r * cols + c] += matrixData[index];
                    }
            }
        }

        std::ostringstream html;
        html << "<h3 style='color:#0f0'>Matrix Output</h3>";
        html << "<div class='matrix-grid'>";
        for (int m = 0; m < mats; m++){
            html << "<div class='matrix-box'>";
            html << "<h3>Matrix" << ( m + 1) << "</h3>";
            for(int r = 0; r < rows; r++){
                html << "<div style='display: flex; gap: 2px;'>";
                for(int c = 0; c < cols; c++){
                    int index = m * rows * cols + r * cols + c;
                html << "<div style='background:#222; color:#0f0; padding:2px; border-radius:2px; text-align:center; min-width:3px;'>"
                 << matrixData[index] << "</div>";
                }
                html << "</div>";
            }
            html << "</div>";
        }
            
        html << "</div>";

        std::ostringstream resultHtml;
        resultHtml << "<h3 style='color:#0f0'>SumResult Matrix</h3>";
        for (int r = 0; r < rows; r++){
            resultHtml <<"<div style='display: flex; gap: 2px;'>";
            for (int c = 0; c < cols; c++){
                int index = r * cols + c;
                resultHtml <<"<div style='background: #222; color:#0f0;padding:2px; border-radius: 2px; text=align:center; min-width:30px;'>"
                 << resultMatrix[index] << "</div>";
            }
            resultHtml <<"</div>";
        }
        std::string matrixSection = html.str();
        std::string resultSection = resultHtml.str();
        EM_ASM({
            console.log("✅ EM_ASM reached inside WASM");
            document.getElementById('output').innerHTML = UTF8ToString($0);
            document.getElementById('resultMatrix').innerHTML = UTF8ToString($1);
        }, matrixSection.c_str(), resultSection.c_str());
    
        delete[]  resultMatrix;
    }
}

 



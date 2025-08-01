#! /bin/bash

mkdir -p build/cppcheck

cppcheck --enable=all \
         --suppress=missingIncludeSystem \
         --suppress='*:/opt/esp/*' \
         --suppress='*:components/arducam/*' \
         --suppress='*:components/arduino/*' \
         --suppress='*:components/esp32servo/*' \
         --suppress='*:components/tinygpsplus/*' \
         --project=build/compile_commands.json \
         -ibuild \
         -i/opt/esp \
         -icomponents/arducam \
         -icomponents/arduino \
         -icomponents/esp32servo \
         -icomponents/tinygpsplus \
         -imanaged_components \
         -itest \
         --std=c++20 \
         --cppcheck-build-dir=build \
         --inline-suppr \
         --xml \
         -j8 \
            2> build/cppcheck/cppcheck_results.xml

if [ "$?" -ne "0" ]; then
  exit
fi

cppcheck-htmlreport --file=build/cppcheck/cppcheck_results.xml \
                    --report-dir=build/reports \
                    --source-dir=. \
                    --source-encoding=UTF-8 \
                    --title=telescope

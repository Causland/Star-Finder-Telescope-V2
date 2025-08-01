#! /bin/bash

rm -rf build/clangtidy
mkdir -p build/clangtidy

cp build/compile_commands.json build/compile_commands.json.sav

sed -i -E 's/-fno-shrink-wrap//g' build/compile_commands.json
sed -i -E 's/-mlong-calls//g' build/compile_commands.json

for f in $(find components/camera \
                components/commands \
                components/gps \
                components/ota \
                components/servo \
                components/task \
                components/telemetry \
                components/utils \
                components/wifi \
                main \
                \( -name '*.cpp' -o -name '*.h' \)); do
  clang-tidy $f -p build \
    --config-file=.clang-tidy \
    -export-fixes=build/clangtidy/$(basename $f).yaml \
    --extra-arg=-target \
    --extra-arg=x86_64-pc-linux-gnu \
    --extra-arg=-fno-color-diagnostics
done

cp build/compile_commands.json.sav build/compile_commands.json
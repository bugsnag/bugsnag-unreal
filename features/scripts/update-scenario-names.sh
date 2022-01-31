#!/bin/bash

set -o errexit

cd "${0%/*}/.."

scenario_names_rb=support/scenario_names.rb
scenario_names_h=fixtures/generic/Source/TestFixture/ScenarioNames.h

echo '# Generated by features/scripts/update-scenario-names.sh' > "$scenario_names_rb"
echo '// Generated by features/scripts/update-scenario-names.sh' > "$scenario_names_h"

echo '$scenario_names = [' >> "$scenario_names_rb"
echo 'static TArray<FString> ScenarioNames = {' >> "$scenario_names_h"

for class in $(perl -ne 'print "$1 " if /class (\S+) : public Scenario/' fixtures/generic/Source/TestFixture/Scenarios/*)
do
  echo "  '$class'," >> "$scenario_names_rb"
  echo "	TEXT(\"$class\")," >> "$scenario_names_h"
done

echo ']' >> "$scenario_names_rb"
echo '};' >> "$scenario_names_h"

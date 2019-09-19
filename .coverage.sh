#!/bin/bash

env

source "${ICI_SRC_PATH}/workspace.sh"

echo "Checking coverage for [$COVERAGE_PKGS]"
coverage_pass=true

for pkg in $COVERAGE_PKGS; do
    echo "Creating coverage for [$pkg]"
    ws=~/target_ws
    extend="/opt/ros/$ROS_DISTRO"
    ici_exec_in_workspace "$extend" "$ws" catkin build $pkg -v --no-deps --catkin-make-args ${pkg}_coverage
    cd $TARGET_REPO_PATH
    echo "Coverage summary for $pkg ----------------------"
    echo "ws: $ws"
    echo "extend $ws"
    #lcov --extract $ws/build/$pkg/${pkg}_coverage.info.cleaned '$ws/src/*' > $ws/build/$pkg/${pkg}_coverage_cleaned.info
    lcov --summary $ws/build/$pkg/${pkg}_coverage.info
    lcov --summary $ws/build/$pkg/${pkg}_coverage.info.cleaned
    echo "---------------------------------------------------"

    line_cov_percentage=$(lcov --summary $ws/build/$pkg/${pkg}_coverage.info.cleaned 2>&1 | grep -Poi "lines\.*: \K[0-9.]*")
    required_coverage="100.0"

    if [ "$line_cov_percentage" != "$required_coverage" ]; then
        echo "$pkg has $line_cov_percentage% line coverage";
        coverages+=("$pkg: $line_cov_percentage% (required: $required_coverage%) \e[${ANSI_RED}m[failed]\e[0m")
        coverage_pass=false
    else
        echo $pkg " has 100.0% line coverage";
        coverages+=("$pkg ($line_cov_percentage%) \e[${ANSI_GREEN}m[pass]\e[0m")
    fi

done

if [ "${coverages// }" != "" ]; then
    echo "Coverage results:"
    for coverages in "${coverages[@]}"; do
        echo -e '  ' $coverages
    done
fi

if ! $coverage_pass; then
  exit 1;
fi
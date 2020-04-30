#!/bin/bash
ret_val=0
for d in */ ; do
    echo "Running git diff "$TRAVIS_PULL_REQUEST_BRANCH" --stat --name-only -- "$d""
    diff="$(git diff "$TRAVIS_PULL_REQUEST_BRANCH" --stat --name-only -- "$d")"
    if [[ $diff ]] && [[ $diff != *"CHANGELOG.rst"* ]]; then
        echo "You changed something in $d but not did not adapt the CHANGELOG.rst. Please fix this!"
        ret_val=1
    fi;
done
echo $ret_val
exit $ret_val
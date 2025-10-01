BUILD_DIR="build"
TEST_DIR="test"
SMOKE_DIR="example"

# TODO: support transformation pass test

unit_test() {
    local path=$1
    local test_file="$1/test.twge"
    local test_result="$1/output.events"
    local test_error="$1/error.log"
    $BUILD_DIR/twgec $test_file 2> error.log
    if [[ -s "$test_file" ]]; then
        if [[ -s $test_error ]] &&  [[ ! -s $test_result ]]; then
            if diff error.log $test_error ; then
                echo "twge test success:    $test_file"
            else
                echo "twge test fail:       $test_file"
            fi
            rm error.log
        elif [[ -s $test_result ]] &&  [[ ! -s $test_error ]]; then
            if [[ ! -s error.log ]]; then
                if diff game.events "$test_result"; then
                    echo "twge test success:    $test_file"
                else
                    echo "twge test fail:       $test_file"
                fi
            else
                cat error.log
                echo "twge test fail:       $test_file"
                rm error.log
            fi
            rm game.events
        else
            echo "Please provide exactly ONE \"output.events\" or \"error.log\" under \"$path\""
        fi
    fi
}

smoke_test() {
    local input_file="$1"
    $BUILD_DIR/twgec "$input_file" 2> error.log
    if [[ ! -s error.log ]]; then
        echo "twge test success:    $input_file"
    else
        cat error.log
        echo "twge test fail:       $input_file"
    fi
    rm game.events
    rm error.log
}

for dir in $(find $TEST_DIR -type d); do
    unit_test $dir
done

for file in $(find $SMOKE_DIR -type f -name "*.twge"); do
    smoke_test $file
done

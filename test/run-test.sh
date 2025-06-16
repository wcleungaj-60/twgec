check_error() {
    local input_file="$1"
    local output_file="$2"
    twgec "$input_file" 2> error.log
    if diff error.log "$output_file"; then
        echo "twge test success:    $1"
    else
        echo "twge test fail:       $1"
    fi
    rm error.log
}

check_output() {
    local input_file="$1"
    local output_file="$2"
    twgec "$input_file" 2> error.log
    if [[ ! -s error.log ]]; then
        if diff game.events "$output_file"; then
            echo "twge test success:    $input_file"
        else
            echo "twge test fail:       $input_file"
        fi
    else
        cat error.log
        echo "twge test fail:       $input_file"
    fi
    rm game.events
    rm error.log
}

check_smoke() {
    local input_file="$1"
    twgec "$input_file" 2> error.log
    if [[ ! -s error.log ]]; then
        echo "twge test success:    $input_file"
    else
        cat error.log
        echo "twge test fail:       $input_file"
    fi
    rm game.events
    rm error.log
}


check_output "test/codegen/actions/addAction/test.twge" "test/codegen/actions/addAction/output.events"
check_output "test/codegen/actions/console/test.twge" "test/codegen/actions/console/output.events"
check_output "test/codegen/metadata/bool/test.twge" "test/codegen/metadata/bool/output.events"
check_output "test/codegen/metadata/int/test.twge" "test/codegen/metadata/int/output.events"
check_output "test/codegen/metadata/listPoint/test.twge" "test/codegen/metadata/listPoint/output.events"
check_output "test/codegen/metadata/string/test.twge" "test/codegen/metadata/string/output.events"
check_error "test/codegen/actions/error_camp/test.twge" "test/codegen/actions/error_camp/error.log"
check_error "test/parser/error_action/test.twge" "test/parser/error_action/error.log"
check_error "test/parser/error_actions/test.twge" "test/parser/error_actions/error.log"
check_error "test/parser/error_block/test.twge" "test/parser/error_block/error.log"
check_error "test/parser/error_module/test.twge" "test/parser/error_module/error.log"
# check_smoke "example/metadata_all.twge"
check_smoke "example/royalVsSkydow.twge"
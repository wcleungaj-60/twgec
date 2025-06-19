check_error() {
    local input_file="$1/test.twge"
    local output_file="$1/error.log"
    twgec "$input_file" 2> error.log
    if diff error.log "$output_file"; then
        echo "twge test success:    $input_file"
    else
        echo "twge test fail:       $input_file"
    fi
    rm error.log
}

check_output() {
    local input_file="$1/test.twge"
    local output_file="$1/output.events"
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


check_output "test/codegen/actions/addAction"
check_output "test/codegen/actions/console"
check_output "test/codegen/metadata/bool"
check_output "test/codegen/metadata/int"
check_output "test/codegen/metadata/listPoint"
check_output "test/codegen/metadata/string"
check_error "test/codegen/actions/error_camp"
check_error "test/codegen/actions/error_undef_redef_arg"
check_error "test/parser/error_action"
check_error "test/parser/error_actions"
check_error "test/parser/error_block"
check_error "test/parser/error_module"
# check_smoke "example/metadata_all.twge"
check_smoke "example/royalVsSkydow.twge"
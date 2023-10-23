#include "vm.h"

#include <iostream>
#include <limits>

#include "util.h"


namespace vm {

vm_state create_vm(bool debug) {
    vm_state state;

    // enable vm debugging
    state.debug = debug;


    register_instruction(state, "PRINT", [](vm_state& vmstate, const item_t /*arg*/) {
        std::cout << vmstate.stack.top() << std::endl;
        return true;
    });


    /************************************************/
    // TODO create instructions
/************************************************/

    // LOAD_CONST : push a number to the stack 
    register_instruction(state, "LOAD_CONST", [](vm_state& vmstate, const item_t number) {
        vmstate.stack.push(number);
        return true;
    });
   

    // EXIT 
    register_instruction(state, "EXIT", [](vm_state& vmstate, const item_t /*arg*/){
        if(vmstate.stack.empty() ){
            throw vm_stackfail("stack is empty");
        }

        return false;
    });

    

    // POP 
    register_instruction(state, "POP", [](vm_state& vmstate, const item_t /*arg*/) {
        // if not enough stack items are available, throw vm_stackfail{"haha"}
        if(vmstate.stack.empty() ){
            std::ostringstream error{"Stack Empty"};
            throw vm_stackfail(error.str());
        }

        item_t top = vmstate.stack.top();
        vmstate.stack.pop();

        return true;
    });

    

    // ADD 
    register_instruction(state, "ADD", [](vm_state& vmstate, const item_t /*arg*/) {
        if(vmstate.stack.size() < 2 ) throw vm_stackfail("Not enough stack");
        
        const auto& TOS = vmstate.stack.top();
        vmstate.stack.pop();
        const auto& TOS1 = vmstate.stack.top();
        vmstate.stack.pop();

        vmstate.stack.push(TOS + TOS1);
        
        return true; 
    });

    

    // DIV 
    register_instruction(state, "DIV", [](vm_state& vmstate, const item_t /*arg*/){
        if(vmstate.stack.size() < 2 ){
            std::ostringstream error{"Not enough stack"};
            throw vm_stackfail(error.str());
        }else if(vmstate.stack.top() == 0){
            std::ostringstream error{"you are trying to divide by zero"};
            throw div_by_zero(error.str());    
        }

        item_t TOS = vmstate.stack.top();
        vmstate.stack.pop();

        item_t TOS1 = vmstate.stack.top();
        vmstate.stack.pop();

        vmstate.stack.push(TOS1 / TOS);
        return true;
    });

    

    // EQ
    register_instruction(state, "EQ", [](vm_state& vmstate, const item_t /*arg*/) {
        if(vmstate.stack.size() < 2 ) throw vm_stackfail("Stck Fail");

        item_t TOS = vmstate.stack.top();
        vmstate.stack.pop();

        item_t TOS1 = vmstate.stack.top();
        vmstate.stack.pop(); 

        if(TOS == TOS1) vmstate.stack.push(1);
        else vmstate.stack.push(0);

        return true;
    });

    

    // NEQ
    register_instruction(state, "NEQ", [](vm_state& vmstate, const item_t /*arg*/) {
        if(vmstate.stack.size() < 2 ) throw vm_stackfail("not enogh stack");

        const auto& TOS = vmstate.stack.top();
        vmstate.stack.pop();

        const auto& TOS1 = vmstate.stack.top();
        vmstate.stack.pop(); 

        if(TOS == TOS1) vmstate.stack.push(0);
        else vmstate.stack.push(1);

        return true;
    });

    

    // DUP
    register_instruction(state, "DUP", [](vm_state& vmstate, const item_t /*arg*/) {
        if(vmstate.stack.empty() ) throw vm_stackfail("stack empty");
        const auto& TOS = vmstate.stack.top();
        
        vmstate.stack.push(TOS);      

        return true;
    });

    

    // JMP
    register_instruction(state, "JMP", [](vm_state& vmstate, const item_t addr) {        
        vmstate.pc = static_cast<size_t>(addr);
        return true;
    });

    

    // JMPZ
    register_instruction(state, "JMPZ", [](vm_state& vmstate, const item_t addr) { 
        if(vmstate.stack.empty() ) throw vm_stackfail("stack empty");
        item_t cond = vmstate.stack.top();
        vmstate.stack.pop();
        if(cond == 0 ){
            vmstate.pc = static_cast<size_t>(addr);
        }

        return true;
    });

    

    // WRITE
    register_instruction(state, "WRITE", [](vm_state& vmstate, const item_t addr) { 
        if(vmstate.stack.empty()) throw vm_stackfail("stack is empty");

        vmstate.outstream << static_cast<item_t>(vmstate.stack.top());
        return true;
    });

    

    // WRITE_CHAR
    register_instruction(state, "WRITE_CHAR", [](vm_state& vmstate, const item_t addr) { 
        if(vmstate.stack.empty()) throw vm_stackfail("stack is empty");
        vmstate.outstream << static_cast<char>(vmstate.stack.top());

        return true;
    });

    
/************************************************/
    // TODO create instructions
/************************************************/
    
    return state;
}


void register_instruction(vm_state& state, std::string_view name, 
                        /* lambda */const op_action_t& action) {
    size_t op_id = state.next_op_id;
    /************************************************/
    //  TODO make instruction available to vm       //
    /************************************************/
    state.next_op_id += 1;
    // add instruction name and id to instruction_id list
    state.instruction_ids.emplace(name, op_id); 
    // add instruction_id and its action to the list 
    state.instruction_actions.emplace(op_id, action);
    // for disassemble 
    state.instruction_names.emplace(op_id, name);

    show_items(name, op_id);
}

void show_items(std::string_view name, size_t op_id){
    std::cout << "name : " << name 
              << " & op_id : " << op_id << "\n";
}


code_t assemble(const vm_state& state, std::string_view input_program) {
    code_t code;

    // convert each line separately
    for (auto& line : util::split(input_program, '\n')) {

        auto line_words = util::split(line, ' ');

        // only support instruction and one argument
        if (line_words.size() >= 3) {
            throw invalid_instruction{std::string{"more than one instruction argument: "} + line};
        }

        // look up instruction id
        auto& op_name = line_words[0];
        auto find_op_id = state.instruction_ids.find(op_name);
        if (find_op_id == std::end(state.instruction_ids)) {
            throw invalid_instruction{std::string{"unknown instruction: "} + op_name};
        }
        op_id_t op_id = find_op_id->second;

        // parse the argument
        item_t argument{0};
        if (line_words.size() == 2) {
            argument = std::stoll(line_words[1]);
        }

        // and save the instruction to the code store
        code.emplace_back(op_id, argument);
    }

    return code;
}


std::tuple<item_t, std::string> run(vm_state& vm, const code_t& code) {
    // to help you debugging the code!
    if (vm.debug) {
        std::cout << "=== running vm ======================" << std::endl;
        std::cout << "disassembly of run code:" << std::endl;
        for (const auto &[op_id, arg] : code) {
            if (not vm.instruction_names.contains(op_id)) {
                std::cout << "could not disassemble - op_id unknown..." << std::endl;
                std::cout << "turning off debug mode." << std::endl;
                vm.debug = false;
                break;
            }
            std::cout << vm.instruction_names[op_id] << " " << arg << std::endl;
        }
        std::cout << "=== end of disassembly" << std::endl << std::endl;
    }

    // execution loop for the machine
    while (true) {

        // SEGFAULT
        if(vm.pc >= code.size()){
            throw vm_segfault{std::string{"invalid address"}};
        }

        auto& [op_id, arg] = code[vm.pc];
        if (vm.debug) {
            std::cout << "-- exec " << vm.instruction_names[op_id] << " arg=" << arg << " at pc=" << vm.pc << std::endl;
        }
        // increase the program counter here so its value can be overwritten
        // by the instruction when it executes!
        vm.pc += 1;

        // TODO execute instruction and stop if the action returns false.
        
        auto& action = vm.instruction_actions[op_id];
        bool running = action(vm, arg);
        if(not running){
            break;
        }
    }

    return {vm.stack.top(), vm.outstream.str()}; // TODO: return tuple(exit value, output text)
}


} // namespace vm

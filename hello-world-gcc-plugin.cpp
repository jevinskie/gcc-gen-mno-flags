#undef NDEBUG
#include <cassert>
#include <cstdio>

// fmt: off
#include <gcc-plugin.h>
#include <plugin-version.h>
// fmt: on

// fmt: off
#include <rtl.h>
// fmt: on

#include <context.h>
#include <diagnostic-core.h>
#include <flags.h>
#include <insn-attr.h>
#include <insn-config.h>
#include <target.h>
#include <tm.h>
// fmt: off
#include <recog.h>
// fmt: on

// Mandatory for all GCC plugins
int plugin_is_GPL_compatible;

// Plugin information
static struct plugin_info iterate_instructions_plugin_info = {.version = "1.0",
                                                              .help = "This is a simple plugin."};

// Function to be executed by the plugin
static void iterate_instructions([[maybe_unused]] void *gcc_data,
                                 [[maybe_unused]] void *user_data) {
    // Print a simple message
    fprintf(stderr, "Iterating over all supported instructions:\n");

    for (unsigned int i = 0; i < NUM_INSN_CODES; ++i) {
        if (insn_data[i].name) {
            fprintf(stderr, "Instruction %d: %s\n", i, insn_data[i].name);
        }
    }
}

// // Function to check if an instruction is valid for the current target
// static bool is_valid_instruction(enum insn_code code) {
//     // if (insn_data[code].insn == nullptr) {
//     //     return false;
//     // }

//     // Attempt to generate the instruction pattern
//     rtx_insn *insn = (*insn_data[code].genfun)();
//     if (insn == nullptr) {
//         return false;
//     }

//     // Simplistic check: Ensure the instruction's pattern is valid
//     return recog_memoized(insn) >= 0;
// }

// // Function to check if a register is a SIMD register
// static bool is_simd_register(unsigned int regno) {
//     for (machine_mode mode = FIRST_PSEUDO_REGISTER; mode < MAX_MACHINE_MODE; ++mode) {
//         if (HARD_REGNO_MODE_OK(regno, mode) && HARD_REGNO_NREGS(regno, mode) > 1) {
//             return true;
//         }
//     }
//     return false;
// }

// // Function to check if an RTX expression modifies FPU or SIMD registers
// static bool modifies_fpu_or_simd(rtx exp) {
//     if (exp == nullptr) {
//         return false;
//     }

//     switch (GET_CODE(exp)) {
//     case SET: {
//         rtx dest = SET_DEST(exp);
//         if (REG_P(dest)) {
//             unsigned int regno = REGNO(dest);
//             if (!GENERAL_REGNO_P(regno) || is_simd_register(regno)) {
//                 return true;
//             }
//         }
//     } break;
//     case PARALLEL:
//         for (int i = 0; i < XVECLEN(exp, 0); ++i) {
//             if (modifies_fpu_or_simd(XVECEXP(exp, 0, i))) {
//                 return true;
//             }
//         }
//         break;
//     default:
//         break;
//     }

//     return false;
// }

// // Function to be executed by the plugin
// static void iterate_instructions([[maybe_unused]] void *gcc_data,
//                                  [[maybe_unused]] void *user_data) {
//     // Print a simple message
//     fprintf(stderr, "Iterating over all supported instructions for the current target:\n");

//     for (unsigned int i = 0; i < NUM_INSN_CODES; ++i) {
//         if (insn_data[i].name && is_valid_instruction((enum insn_code)i)) {
//             if (modifies_fpu_or_simd(PATTERN(insn_data[i].genfun()))) {
//                 fprintf(stderr, "Instruction %d: %s modifies FPU or SIMD registers\n", i,
//                         insn_data[i].name);
//             } else {
//                 fprintf(stderr, "Instruction %d: %s\n", i, insn_data[i].name);
//             }
//         }
//     }
// }

// Plugin initialization
int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
    // Check GCC version compatibility
    if (!plugin_default_version_check(version, &gcc_version)) {
        error("GCC version mismatch");
        return 1;
    }

    // Register the plugin info
    register_callback(plugin_info->base_name, PLUGIN_INFO, nullptr,
                      reinterpret_cast<void *>(&iterate_instructions_plugin_info));
    // Register the callback function for the plugin
    register_callback(plugin_info->base_name, PLUGIN_ALL_PASSES_START, &iterate_instructions,
                      nullptr);
    return 0;
}

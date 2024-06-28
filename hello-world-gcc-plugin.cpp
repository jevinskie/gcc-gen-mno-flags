#undef NDEBUG
#include <cassert>

// fmt: off
#include <gcc-plugin.h>
// fmt: on

#include <context.h>
#include <cstdio>
#include <diagnostic-core.h>
#include <plugin-version.h>
#include <tree-pass.h>
#include <tree.h>

// Mandatory for all GCC plugins
int plugin_is_GPL_compatible = 1;

// Plugin information
static struct plugin_info hello_world_plugin_info = {
    .version = "1.0", .help = "This is a simple Hello World plugin."};

// Function to be executed by the plugin
void hello_world_function([[maybe_unused]] void *gcc_data, [[maybe_unused]] void *user_data) {
    // Print a simple message
    fprintf(stderr, "Hello, World from GCC plugin!\n");
}

// Plugin information
int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
    // Check GCC version compatibility
    if (!plugin_default_version_check(version, &gcc_version)) {
        error("GCC version mismatch");
        return 1;
    }

    // Register the plugin info
    register_callback(plugin_info->base_name, PLUGIN_INFO, nullptr,
                      reinterpret_cast<void *>(&hello_world_plugin_info));
    // Register the callback function for the plugin
    register_callback(plugin_info->base_name, PLUGIN_FINISH, &hello_world_function, nullptr);
    return 0;
}

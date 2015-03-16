#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xae4b0ceb, "module_layout" },
	{ 0x15692c87, "param_ops_int" },
	{ 0xd3a204c1, "platform_driver_unregister" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xefd55b63, "platform_driver_register" },
	{ 0x53968437, "dev_warn" },
	{ 0x782011c7, "devm_pinctrl_put" },
	{ 0x86c8ad2e, "malloc_sizes" },
	{ 0x7e80c676, "sysfs_create_files" },
	{ 0x3b65fd70, "__uio_register_device" },
	{ 0x733c3b54, "kasprintf" },
	{ 0xc3577ebb, "platform_get_irq" },
	{ 0xd3094c12, "of_property_read_u32_array" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0x3f098b6b, "gen_pool_virt_to_phys" },
	{ 0xd01009f1, "platform_get_resource" },
	{ 0x75a26fbd, "dev_err" },
	{ 0x3d38ea6d, "of_property_read_string_index" },
	{ 0xf4fe49e1, "devm_gpio_request_one" },
	{ 0x3757c9b, "of_get_named_gpio_flags" },
	{ 0x7985fab5, "of_property_count_strings" },
	{ 0xacd0a79a, "of_gpio_named_count" },
	{ 0x16fdefd2, "of_get_next_child" },
	{ 0x1eac9fb, "pinctrl_select_state" },
	{ 0x3f91b9ef, "pinctrl_lookup_state" },
	{ 0x13371c7b, "devm_pinctrl_get" },
	{ 0xe5524eab, "of_address_to_resource" },
	{ 0x64119029, "__pm_runtime_resume" },
	{ 0xafde83a0, "pm_runtime_enable" },
	{ 0x8758668a, "kmem_cache_alloc_trace" },
	{ 0xaaebf074, "dev_set_drvdata" },
	{ 0x2e1ca751, "clk_put" },
	{ 0xc2165d85, "__arm_iounmap" },
	{ 0x37a0cba, "kfree" },
	{ 0x9ac7e6f6, "uio_unregister_device" },
	{ 0xa1ac6d78, "sysfs_remove_files" },
	{ 0x361bbc23, "arm_dma_ops" },
	{ 0xc22f24be, "dev_get_drvdata" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xac8f37b2, "outer_cache" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cti,pruss-v1*");
MODULE_ALIAS("of:N*T*Cti,pruss-v2*");

MODULE_INFO(srcversion, "E91FD4564FBEB2A8E374C16");

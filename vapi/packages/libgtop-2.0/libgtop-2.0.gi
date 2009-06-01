<?xml version="1.0"?>
<api version="1.0">
	<namespace name="GTop">
		<struct name="glibtop">
			<method name="call_l" symbol="glibtop_call_l">
				<return-type type="void*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="command" type="unsigned"/>
					<parameter name="send_size" type="size_t"/>
					<parameter name="send_buf" type="void*"/>
					<parameter name="recv_size" type="size_t"/>
					<parameter name="recv_buf" type="void*"/>
				</parameters>
			</method>
			<method name="call_s" symbol="glibtop_call_s">
				<return-type type="void*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="command" type="unsigned"/>
					<parameter name="send_size" type="size_t"/>
					<parameter name="send_buf" type="void*"/>
					<parameter name="recv_size" type="size_t"/>
					<parameter name="recv_buf" type="void*"/>
				</parameters>
			</method>
			<method name="close" symbol="glibtop_close">
				<return-type type="void"/>
			</method>
			<method name="close_p" symbol="glibtop_close_p">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
				</parameters>
			</method>
			<method name="close_r" symbol="glibtop_close_r">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
				</parameters>
			</method>
			<method name="close_s" symbol="glibtop_close_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
				</parameters>
			</method>
			<method name="get_cpu" symbol="glibtop_get_cpu">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_cpu*"/>
				</parameters>
			</method>
			<method name="get_cpu_l" symbol="glibtop_get_cpu_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_cpu*"/>
				</parameters>
			</method>
			<method name="get_cpu_s" symbol="glibtop_get_cpu_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_cpu*"/>
				</parameters>
			</method>
			<method name="get_fsusage" symbol="glibtop_get_fsusage">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_fsusage*"/>
					<parameter name="mount_dir" type="char*"/>
				</parameters>
			</method>
			<method name="get_fsusage_l" symbol="glibtop_get_fsusage_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_fsusage*"/>
					<parameter name="mount_dir" type="char*"/>
				</parameters>
			</method>
			<method name="get_fsusage_s" symbol="glibtop_get_fsusage_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_fsusage*"/>
					<parameter name="mount_dir" type="char*"/>
				</parameters>
			</method>
			<method name="get_loadavg" symbol="glibtop_get_loadavg">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_loadavg*"/>
				</parameters>
			</method>
			<method name="get_loadavg_l" symbol="glibtop_get_loadavg_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_loadavg*"/>
				</parameters>
			</method>
			<method name="get_loadavg_s" symbol="glibtop_get_loadavg_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_loadavg*"/>
				</parameters>
			</method>
			<method name="get_mem" symbol="glibtop_get_mem">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_mem*"/>
				</parameters>
			</method>
			<method name="get_mem_l" symbol="glibtop_get_mem_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_mem*"/>
				</parameters>
			</method>
			<method name="get_mem_s" symbol="glibtop_get_mem_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_mem*"/>
				</parameters>
			</method>
			<method name="get_mountlist" symbol="glibtop_get_mountlist">
				<return-type type="glibtop_mountentry*"/>
				<parameters>
					<parameter name="buf" type="glibtop_mountlist*"/>
					<parameter name="all_fs" type="int"/>
				</parameters>
			</method>
			<method name="get_mountlist_l" symbol="glibtop_get_mountlist_l">
				<return-type type="glibtop_mountentry*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_mountlist*"/>
					<parameter name="all_fs" type="int"/>
				</parameters>
			</method>
			<method name="get_mountlist_s" symbol="glibtop_get_mountlist_s">
				<return-type type="glibtop_mountentry*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_mountlist*"/>
					<parameter name="all_fs" type="int"/>
				</parameters>
			</method>
			<method name="get_msg_limits" symbol="glibtop_get_msg_limits">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_msg_limits*"/>
				</parameters>
			</method>
			<method name="get_msg_limits_l" symbol="glibtop_get_msg_limits_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_msg_limits*"/>
				</parameters>
			</method>
			<method name="get_msg_limits_s" symbol="glibtop_get_msg_limits_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_msg_limits*"/>
				</parameters>
			</method>
			<method name="get_netlist" symbol="glibtop_get_netlist">
				<return-type type="char**"/>
				<parameters>
					<parameter name="buf" type="glibtop_netlist*"/>
				</parameters>
			</method>
			<method name="get_netlist_l" symbol="glibtop_get_netlist_l">
				<return-type type="char**"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_netlist*"/>
				</parameters>
			</method>
			<method name="get_netlist_s" symbol="glibtop_get_netlist_s">
				<return-type type="char**"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_netlist*"/>
				</parameters>
			</method>
			<method name="get_netload" symbol="glibtop_get_netload">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_netload*"/>
					<parameter name="interface" type="char*"/>
				</parameters>
			</method>
			<method name="get_netload_l" symbol="glibtop_get_netload_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_netload*"/>
					<parameter name="interface" type="char*"/>
				</parameters>
			</method>
			<method name="get_netload_s" symbol="glibtop_get_netload_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_netload*"/>
					<parameter name="interface" type="char*"/>
				</parameters>
			</method>
			<method name="get_parameter_l" symbol="glibtop_get_parameter_l">
				<return-type type="size_t"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="parameter" type="unsigned"/>
					<parameter name="data_ptr" type="void*"/>
					<parameter name="data_size" type="size_t"/>
				</parameters>
			</method>
			<method name="get_ppp" symbol="glibtop_get_ppp">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_ppp*"/>
					<parameter name="device" type="unsigned"/>
				</parameters>
			</method>
			<method name="get_ppp_l" symbol="glibtop_get_ppp_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_ppp*"/>
					<parameter name="device" type="unsigned"/>
				</parameters>
			</method>
			<method name="get_ppp_s" symbol="glibtop_get_ppp_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_ppp*"/>
					<parameter name="device" type="unsigned"/>
				</parameters>
			</method>
			<method name="get_proc_affinity" symbol="glibtop_get_proc_affinity">
				<return-type type="guint16*"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_affinity*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_affinity_l" symbol="glibtop_get_proc_affinity_l">
				<return-type type="guint16*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_affinity*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_affinity_s" symbol="glibtop_get_proc_affinity_s">
				<return-type type="guint16*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_affinity*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_args" symbol="glibtop_get_proc_args">
				<return-type type="char*"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_args*"/>
					<parameter name="pid" type="pid_t"/>
					<parameter name="max_len" type="unsigned"/>
				</parameters>
			</method>
			<method name="get_proc_args_l" symbol="glibtop_get_proc_args_l">
				<return-type type="char*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_args*"/>
					<parameter name="pid" type="pid_t"/>
					<parameter name="max_len" type="unsigned"/>
				</parameters>
			</method>
			<method name="get_proc_args_s" symbol="glibtop_get_proc_args_s">
				<return-type type="char*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_args*"/>
					<parameter name="pid" type="pid_t"/>
					<parameter name="max_len" type="unsigned"/>
				</parameters>
			</method>
			<method name="get_proc_argv" symbol="glibtop_get_proc_argv">
				<return-type type="char**"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_args*"/>
					<parameter name="pid" type="pid_t"/>
					<parameter name="max_len" type="unsigned"/>
				</parameters>
			</method>
			<method name="get_proc_argv_l" symbol="glibtop_get_proc_argv_l">
				<return-type type="char**"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_args*"/>
					<parameter name="pid" type="pid_t"/>
					<parameter name="max_len" type="unsigned"/>
				</parameters>
			</method>
			<method name="get_proc_kernel" symbol="glibtop_get_proc_kernel">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_kernel*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_kernel_l" symbol="glibtop_get_proc_kernel_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_kernel*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_kernel_s" symbol="glibtop_get_proc_kernel_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_kernel*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_map" symbol="glibtop_get_proc_map">
				<return-type type="glibtop_map_entry*"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_map*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_map_l" symbol="glibtop_get_proc_map_l">
				<return-type type="glibtop_map_entry*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_map*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_map_s" symbol="glibtop_get_proc_map_s">
				<return-type type="glibtop_map_entry*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_map*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_mem" symbol="glibtop_get_proc_mem">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_mem*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_mem_l" symbol="glibtop_get_proc_mem_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_mem*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_mem_s" symbol="glibtop_get_proc_mem_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_mem*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_open_files" symbol="glibtop_get_proc_open_files">
				<return-type type="glibtop_open_files_entry*"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_open_files*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_open_files_l" symbol="glibtop_get_proc_open_files_l">
				<return-type type="glibtop_open_files_entry*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_open_files*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_open_files_s" symbol="glibtop_get_proc_open_files_s">
				<return-type type="glibtop_open_files_entry*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_open_files*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_segment" symbol="glibtop_get_proc_segment">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_segment*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_segment_l" symbol="glibtop_get_proc_segment_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_segment*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_segment_s" symbol="glibtop_get_proc_segment_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_segment*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_signal" symbol="glibtop_get_proc_signal">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_signal*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_signal_l" symbol="glibtop_get_proc_signal_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_signal*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_signal_s" symbol="glibtop_get_proc_signal_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_signal*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_state" symbol="glibtop_get_proc_state">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_state*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_state_l" symbol="glibtop_get_proc_state_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_state*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_state_s" symbol="glibtop_get_proc_state_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_state*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_time" symbol="glibtop_get_proc_time">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_time*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_time_l" symbol="glibtop_get_proc_time_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_time*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_time_s" symbol="glibtop_get_proc_time_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_time*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_uid" symbol="glibtop_get_proc_uid">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_uid*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_uid_l" symbol="glibtop_get_proc_uid_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_uid*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_uid_s" symbol="glibtop_get_proc_uid_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_uid*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_wd" symbol="glibtop_get_proc_wd">
				<return-type type="char**"/>
				<parameters>
					<parameter name="buf" type="glibtop_proc_wd*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_wd_l" symbol="glibtop_get_proc_wd_l">
				<return-type type="char**"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_wd*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proc_wd_s" symbol="glibtop_get_proc_wd_s">
				<return-type type="char**"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proc_wd*"/>
					<parameter name="pid" type="pid_t"/>
				</parameters>
			</method>
			<method name="get_proclist" symbol="glibtop_get_proclist">
				<return-type type="pid_t*"/>
				<parameters>
					<parameter name="buf" type="glibtop_proclist*"/>
					<parameter name="which" type="gint64"/>
					<parameter name="arg" type="gint64"/>
				</parameters>
			</method>
			<method name="get_proclist_l" symbol="glibtop_get_proclist_l">
				<return-type type="pid_t*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proclist*"/>
					<parameter name="which" type="gint64"/>
					<parameter name="arg" type="gint64"/>
				</parameters>
			</method>
			<method name="get_proclist_s" symbol="glibtop_get_proclist_s">
				<return-type type="pid_t*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_proclist*"/>
					<parameter name="which" type="gint64"/>
					<parameter name="arg" type="gint64"/>
				</parameters>
			</method>
			<method name="get_sem_limits" symbol="glibtop_get_sem_limits">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_sem_limits*"/>
				</parameters>
			</method>
			<method name="get_sem_limits_l" symbol="glibtop_get_sem_limits_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_sem_limits*"/>
				</parameters>
			</method>
			<method name="get_sem_limits_s" symbol="glibtop_get_sem_limits_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_sem_limits*"/>
				</parameters>
			</method>
			<method name="get_shm_limits" symbol="glibtop_get_shm_limits">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_shm_limits*"/>
				</parameters>
			</method>
			<method name="get_shm_limits_l" symbol="glibtop_get_shm_limits_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_shm_limits*"/>
				</parameters>
			</method>
			<method name="get_shm_limits_s" symbol="glibtop_get_shm_limits_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_shm_limits*"/>
				</parameters>
			</method>
			<method name="get_swap" symbol="glibtop_get_swap">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_swap*"/>
				</parameters>
			</method>
			<method name="get_swap_l" symbol="glibtop_get_swap_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_swap*"/>
				</parameters>
			</method>
			<method name="get_swap_s" symbol="glibtop_get_swap_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_swap*"/>
				</parameters>
			</method>
			<method name="get_sysdeps" symbol="glibtop_get_sysdeps">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_sysdeps*"/>
				</parameters>
			</method>
			<method name="get_sysdeps_r" symbol="glibtop_get_sysdeps_r">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_sysdeps*"/>
				</parameters>
			</method>
			<method name="get_sysinfo" symbol="glibtop_get_sysinfo">
				<return-type type="glibtop_sysinfo*"/>
			</method>
			<method name="get_sysinfo_s" symbol="glibtop_get_sysinfo_s">
				<return-type type="glibtop_sysinfo*"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
				</parameters>
			</method>
			<method name="get_uptime" symbol="glibtop_get_uptime">
				<return-type type="void"/>
				<parameters>
					<parameter name="buf" type="glibtop_uptime*"/>
				</parameters>
			</method>
			<method name="get_uptime_l" symbol="glibtop_get_uptime_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_uptime*"/>
				</parameters>
			</method>
			<method name="get_uptime_s" symbol="glibtop_get_uptime_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="buf" type="glibtop_uptime*"/>
				</parameters>
			</method>
			<method name="init" symbol="glibtop_init">
				<return-type type="glibtop*"/>
			</method>
			<method name="init_p" symbol="glibtop_init_p">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="features" type="unsigned"/>
					<parameter name="flags" type="unsigned"/>
				</parameters>
			</method>
			<method name="init_r" symbol="glibtop_init_r">
				<return-type type="glibtop*"/>
				<parameters>
					<parameter name="server_ptr" type="glibtop**"/>
					<parameter name="features" type="unsigned"/>
					<parameter name="flags" type="unsigned"/>
				</parameters>
			</method>
			<method name="init_s" symbol="glibtop_init_s">
				<return-type type="glibtop*"/>
				<parameters>
					<parameter name="server_ptr" type="glibtop**"/>
					<parameter name="features" type="unsigned"/>
					<parameter name="flags" type="unsigned"/>
				</parameters>
			</method>
			<method name="internet_addr" symbol="glibtop_internet_addr">
				<return-type type="long"/>
				<parameters>
					<parameter name="host" type="char*"/>
				</parameters>
			</method>
			<method name="make_connection" symbol="glibtop_make_connection">
				<return-type type="int"/>
				<parameters>
					<parameter name="hostarg" type="char*"/>
					<parameter name="portarg" type="int"/>
					<parameter name="s" type="int*"/>
				</parameters>
			</method>
			<method name="open_l" symbol="glibtop_open_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="program_name" type="char*"/>
					<parameter name="features" type="unsigned"/>
					<parameter name="flags" type="unsigned"/>
				</parameters>
			</method>
			<method name="open_p" symbol="glibtop_open_p">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="program_name" type="char*"/>
					<parameter name="features" type="unsigned"/>
					<parameter name="flags" type="unsigned"/>
				</parameters>
			</method>
			<method name="open_s" symbol="glibtop_open_s">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="program_name" type="char*"/>
					<parameter name="features" type="unsigned"/>
					<parameter name="flags" type="unsigned"/>
				</parameters>
			</method>
			<method name="set_parameter_l" symbol="glibtop_set_parameter_l">
				<return-type type="void"/>
				<parameters>
					<parameter name="server" type="glibtop*"/>
					<parameter name="parameter" type="unsigned"/>
					<parameter name="data_ptr" type="void*"/>
					<parameter name="data_size" type="size_t"/>
				</parameters>
			</method>
			<field name="flags" type="unsigned"/>
			<field name="method" type="unsigned"/>
			<field name="error_method" type="unsigned"/>
			<field name="input" type="int[]"/>
			<field name="output" type="int[]"/>
			<field name="socket" type="int"/>
			<field name="ncpu" type="int"/>
			<field name="real_ncpu" type="int"/>
			<field name="os_version_code" type="unsigned"/>
			<field name="name" type="char*"/>
			<field name="server_command" type="char*"/>
			<field name="server_host" type="char*"/>
			<field name="server_user" type="char*"/>
			<field name="server_rsh" type="char*"/>
			<field name="features" type="unsigned"/>
			<field name="server_port" type="unsigned"/>
			<field name="sysdeps" type="glibtop_sysdeps"/>
			<field name="required" type="glibtop_sysdeps"/>
			<field name="pid" type="pid_t"/>
		</struct>
		<struct name="glibtop_command">
			<field name="command" type="guint64"/>
			<field name="size" type="guint64"/>
			<field name="data_size" type="guint64"/>
			<field name="parameter" type="char[]"/>
		</struct>
		<struct name="glibtop_cpu">
			<field name="flags" type="guint64"/>
			<field name="total" type="guint64"/>
			<field name="user" type="guint64"/>
			<field name="nice" type="guint64"/>
			<field name="sys" type="guint64"/>
			<field name="idle" type="guint64"/>
			<field name="iowait" type="guint64"/>
			<field name="irq" type="guint64"/>
			<field name="softirq" type="guint64"/>
			<field name="frequency" type="guint64"/>
			<field name="xcpu_total" type="guint64[]"/>
			<field name="xcpu_user" type="guint64[]"/>
			<field name="xcpu_nice" type="guint64[]"/>
			<field name="xcpu_sys" type="guint64[]"/>
			<field name="xcpu_idle" type="guint64[]"/>
			<field name="xcpu_iowait" type="guint64[]"/>
			<field name="xcpu_irq" type="guint64[]"/>
			<field name="xcpu_softirq" type="guint64[]"/>
			<field name="xcpu_flags" type="guint64"/>
		</struct>
		<struct name="glibtop_entry">
			<field name="labels" type="GPtrArray*"/>
			<field name="values" type="GHashTable*"/>
			<field name="descriptions" type="GHashTable*"/>
		</struct>
		<struct name="glibtop_fsusage">
			<field name="flags" type="guint64"/>
			<field name="blocks" type="guint64"/>
			<field name="bfree" type="guint64"/>
			<field name="bavail" type="guint64"/>
			<field name="files" type="guint64"/>
			<field name="ffree" type="guint64"/>
			<field name="block_size" type="guint32"/>
			<field name="read" type="guint64"/>
			<field name="write" type="guint64"/>
		</struct>
		<struct name="glibtop_loadavg">
			<field name="flags" type="guint64"/>
			<field name="loadavg" type="double[]"/>
			<field name="nr_running" type="guint64"/>
			<field name="nr_tasks" type="guint64"/>
			<field name="last_pid" type="guint64"/>
		</struct>
		<struct name="glibtop_machine">
			<field name="last_pid" type="pid_t"/>
			<field name="no_update" type="int"/>
			<field name="fd_stat" type="int"/>
			<field name="fd_meminfo" type="int"/>
			<field name="fd_loadavg" type="int"/>
			<field name="proc_stat" type="char[]"/>
			<field name="proc_statm" type="char[]"/>
			<field name="proc_status" type="char[]"/>
		</struct>
		<struct name="glibtop_map_entry">
			<field name="flags" type="guint64"/>
			<field name="start" type="guint64"/>
			<field name="end" type="guint64"/>
			<field name="offset" type="guint64"/>
			<field name="perm" type="guint64"/>
			<field name="inode" type="guint64"/>
			<field name="device" type="guint64"/>
			<field name="size" type="guint64"/>
			<field name="rss" type="guint64"/>
			<field name="shared_clean" type="guint64"/>
			<field name="shared_dirty" type="guint64"/>
			<field name="private_clean" type="guint64"/>
			<field name="private_dirty" type="guint64"/>
			<field name="filename" type="char[]"/>
		</struct>
		<struct name="glibtop_mem">
			<field name="flags" type="guint64"/>
			<field name="total" type="guint64"/>
			<field name="used" type="guint64"/>
			<field name="free" type="guint64"/>
			<field name="shared" type="guint64"/>
			<field name="buffer" type="guint64"/>
			<field name="cached" type="guint64"/>
			<field name="user" type="guint64"/>
			<field name="locked" type="guint64"/>
		</struct>
		<struct name="glibtop_mountentry">
			<field name="dev" type="guint64"/>
			<field name="devname" type="char[]"/>
			<field name="mountdir" type="char[]"/>
			<field name="type" type="char[]"/>
		</struct>
		<struct name="glibtop_mountlist">
			<field name="flags" type="guint64"/>
			<field name="number" type="guint64"/>
			<field name="total" type="guint64"/>
			<field name="size" type="guint64"/>
		</struct>
		<struct name="glibtop_msg_limits">
			<field name="flags" type="guint64"/>
			<field name="msgpool" type="guint64"/>
			<field name="msgmap" type="guint64"/>
			<field name="msgmax" type="guint64"/>
			<field name="msgmnb" type="guint64"/>
			<field name="msgmni" type="guint64"/>
			<field name="msgssz" type="guint64"/>
			<field name="msgtql" type="guint64"/>
		</struct>
		<struct name="glibtop_netlist">
			<field name="flags" type="guint64"/>
			<field name="number" type="guint32"/>
		</struct>
		<struct name="glibtop_netload">
			<field name="flags" type="guint64"/>
			<field name="if_flags" type="guint64"/>
			<field name="mtu" type="guint32"/>
			<field name="subnet" type="guint32"/>
			<field name="address" type="guint32"/>
			<field name="packets_in" type="guint64"/>
			<field name="packets_out" type="guint64"/>
			<field name="packets_total" type="guint64"/>
			<field name="bytes_in" type="guint64"/>
			<field name="bytes_out" type="guint64"/>
			<field name="bytes_total" type="guint64"/>
			<field name="errors_in" type="guint64"/>
			<field name="errors_out" type="guint64"/>
			<field name="errors_total" type="guint64"/>
			<field name="collisions" type="guint64"/>
			<field name="address6" type="guint8[]"/>
			<field name="prefix6" type="guint8[]"/>
			<field name="scope6" type="guint8"/>
			<field name="hwaddress" type="guint8[]"/>
		</struct>
		<struct name="glibtop_open_files_entry">
			<field name="fd" type="int"/>
			<field name="type" type="guint16"/>
			<field name="info" type="gpointer"/>
		</struct>
		<struct name="glibtop_ppp">
			<field name="flags" type="guint64"/>
			<field name="state" type="guint64"/>
			<field name="bytes_in" type="guint64"/>
			<field name="bytes_out" type="guint64"/>
		</struct>
		<struct name="glibtop_proc_affinity">
			<field name="flags" type="guint64"/>
			<field name="number" type="guint32"/>
			<field name="all" type="gboolean"/>
		</struct>
		<struct name="glibtop_proc_args">
			<field name="flags" type="guint64"/>
			<field name="size" type="guint64"/>
		</struct>
		<struct name="glibtop_proc_kernel">
			<field name="flags" type="guint64"/>
			<field name="k_flags" type="guint64"/>
			<field name="min_flt" type="guint64"/>
			<field name="maj_flt" type="guint64"/>
			<field name="cmin_flt" type="guint64"/>
			<field name="cmaj_flt" type="guint64"/>
			<field name="kstk_esp" type="guint64"/>
			<field name="kstk_eip" type="guint64"/>
			<field name="nwchan" type="guint64"/>
			<field name="wchan" type="char[]"/>
		</struct>
		<struct name="glibtop_proc_map">
			<field name="flags" type="guint64"/>
			<field name="number" type="guint64"/>
			<field name="total" type="guint64"/>
			<field name="size" type="guint64"/>
		</struct>
		<struct name="glibtop_proc_mem">
			<field name="flags" type="guint64"/>
			<field name="size" type="guint64"/>
			<field name="vsize" type="guint64"/>
			<field name="resident" type="guint64"/>
			<field name="share" type="guint64"/>
			<field name="rss" type="guint64"/>
			<field name="rss_rlim" type="guint64"/>
		</struct>
		<struct name="glibtop_proc_open_files">
			<field name="flags" type="guint64"/>
			<field name="number" type="guint64"/>
			<field name="total" type="guint64"/>
			<field name="size" type="guint64"/>
		</struct>
		<struct name="glibtop_proc_segment">
			<field name="flags" type="guint64"/>
			<field name="text_rss" type="guint64"/>
			<field name="shlib_rss" type="guint64"/>
			<field name="data_rss" type="guint64"/>
			<field name="stack_rss" type="guint64"/>
			<field name="dirty_size" type="guint64"/>
			<field name="start_code" type="guint64"/>
			<field name="end_code" type="guint64"/>
			<field name="start_stack" type="guint64"/>
		</struct>
		<struct name="glibtop_proc_signal">
			<field name="flags" type="guint64"/>
			<field name="signal" type="guint64[]"/>
			<field name="blocked" type="guint64[]"/>
			<field name="sigignore" type="guint64[]"/>
			<field name="sigcatch" type="guint64[]"/>
		</struct>
		<struct name="glibtop_proc_state">
			<field name="flags" type="guint64"/>
			<field name="cmd" type="char[]"/>
			<field name="state" type="unsigned"/>
			<field name="uid" type="int"/>
			<field name="gid" type="int"/>
			<field name="ruid" type="int"/>
			<field name="rgid" type="int"/>
			<field name="has_cpu" type="int"/>
			<field name="processor" type="int"/>
			<field name="last_processor" type="int"/>
		</struct>
		<struct name="glibtop_proc_time">
			<field name="flags" type="guint64"/>
			<field name="start_time" type="guint64"/>
			<field name="rtime" type="guint64"/>
			<field name="utime" type="guint64"/>
			<field name="stime" type="guint64"/>
			<field name="cutime" type="guint64"/>
			<field name="cstime" type="guint64"/>
			<field name="timeout" type="guint64"/>
			<field name="it_real_value" type="guint64"/>
			<field name="frequency" type="guint64"/>
			<field name="xcpu_utime" type="guint64[]"/>
			<field name="xcpu_stime" type="guint64[]"/>
		</struct>
		<struct name="glibtop_proc_uid">
			<field name="flags" type="guint64"/>
			<field name="uid" type="gint32"/>
			<field name="euid" type="gint32"/>
			<field name="gid" type="gint32"/>
			<field name="egid" type="gint32"/>
			<field name="suid" type="gint32"/>
			<field name="sgid" type="gint32"/>
			<field name="fsuid" type="gint32"/>
			<field name="fsgid" type="gint32"/>
			<field name="pid" type="gint32"/>
			<field name="ppid" type="gint32"/>
			<field name="pgrp" type="gint32"/>
			<field name="session" type="gint32"/>
			<field name="tty" type="gint32"/>
			<field name="tpgid" type="gint32"/>
			<field name="priority" type="gint32"/>
			<field name="nice" type="gint32"/>
			<field name="ngroups" type="gint32"/>
			<field name="groups" type="gint32[]"/>
		</struct>
		<struct name="glibtop_proc_wd">
			<field name="flags" type="guint64"/>
			<field name="number" type="guint32"/>
			<field name="root" type="char[]"/>
			<field name="exe" type="char[]"/>
		</struct>
		<struct name="glibtop_proclist">
			<field name="flags" type="guint64"/>
			<field name="number" type="guint64"/>
			<field name="total" type="guint64"/>
			<field name="size" type="guint64"/>
		</struct>
		<struct name="glibtop_response">
			<field name="offset" type="gint64"/>
			<field name="size" type="guint64"/>
			<field name="data_size" type="guint64"/>
			<field name="u" type="glibtop_response_union"/>
		</struct>
		<struct name="glibtop_sem_limits">
			<field name="flags" type="guint64"/>
			<field name="semmap" type="guint64"/>
			<field name="semmni" type="guint64"/>
			<field name="semmns" type="guint64"/>
			<field name="semmnu" type="guint64"/>
			<field name="semmsl" type="guint64"/>
			<field name="semopm" type="guint64"/>
			<field name="semume" type="guint64"/>
			<field name="semusz" type="guint64"/>
			<field name="semvmx" type="guint64"/>
			<field name="semaem" type="guint64"/>
		</struct>
		<struct name="glibtop_shm_limits">
			<field name="flags" type="guint64"/>
			<field name="shmmax" type="guint64"/>
			<field name="shmmin" type="guint64"/>
			<field name="shmmni" type="guint64"/>
			<field name="shmseg" type="guint64"/>
			<field name="shmall" type="guint64"/>
		</struct>
		<struct name="glibtop_signame">
			<field name="number" type="int"/>
			<field name="name" type="char*"/>
			<field name="label" type="char*"/>
		</struct>
		<struct name="glibtop_swap">
			<field name="flags" type="guint64"/>
			<field name="total" type="guint64"/>
			<field name="used" type="guint64"/>
			<field name="free" type="guint64"/>
			<field name="pagein" type="guint64"/>
			<field name="pageout" type="guint64"/>
		</struct>
		<struct name="glibtop_sysdeps">
			<field name="flags" type="guint64"/>
			<field name="features" type="guint64"/>
			<field name="cpu" type="guint64"/>
			<field name="mem" type="guint64"/>
			<field name="swap" type="guint64"/>
			<field name="uptime" type="guint64"/>
			<field name="loadavg" type="guint64"/>
			<field name="shm_limits" type="guint64"/>
			<field name="msg_limits" type="guint64"/>
			<field name="sem_limits" type="guint64"/>
			<field name="proclist" type="guint64"/>
			<field name="proc_state" type="guint64"/>
			<field name="proc_uid" type="guint64"/>
			<field name="proc_mem" type="guint64"/>
			<field name="proc_time" type="guint64"/>
			<field name="proc_signal" type="guint64"/>
			<field name="proc_kernel" type="guint64"/>
			<field name="proc_segment" type="guint64"/>
			<field name="proc_args" type="guint64"/>
			<field name="proc_map" type="guint64"/>
			<field name="proc_open_files" type="guint64"/>
			<field name="mountlist" type="guint64"/>
			<field name="fsusage" type="guint64"/>
			<field name="netlist" type="guint64"/>
			<field name="netload" type="guint64"/>
			<field name="ppp" type="guint64"/>
			<field name="proc_wd" type="guint64"/>
			<field name="proc_affinity" type="guint64"/>
		</struct>
		<struct name="glibtop_sysinfo">
			<field name="flags" type="guint64"/>
			<field name="ncpu" type="guint64"/>
			<field name="cpuinfo" type="glibtop_entry[]"/>
		</struct>
		<struct name="glibtop_uptime">
			<field name="flags" type="guint64"/>
			<field name="uptime" type="double"/>
			<field name="idletime" type="double"/>
			<field name="boot_time" type="guint64"/>
		</struct>
		<constant name="AUTH_NAMESZ" type="int" value="15"/>
		<constant name="AUTH_TIMEOUT" type="int" value="15"/>
		<constant name="CONN_INTERNET" type="int" value="1"/>
		<constant name="CONN_IPC" type="int" value="2"/>
		<constant name="CONN_UNIX" type="int" value="0"/>
		<constant name="DEFAULT_PORT" type="int" value="21490"/>
		<constant name="DEFAUTH_NAME" type="char*" value="GNU-SECURE"/>
		<constant name="EOT_STR" type="char*" value="&#x4;"/>
		<constant name="FALSE" type="int" value="0"/>
		<constant name="GLIBTOP_CMND_CPU" type="int" value="2"/>
		<constant name="GLIBTOP_CMND_FSUSAGE" type="int" value="21"/>
		<constant name="GLIBTOP_CMND_LOADAVG" type="int" value="6"/>
		<constant name="GLIBTOP_CMND_MEM" type="int" value="3"/>
		<constant name="GLIBTOP_CMND_MOUNTLIST" type="int" value="20"/>
		<constant name="GLIBTOP_CMND_MSG_LIMITS" type="int" value="8"/>
		<constant name="GLIBTOP_CMND_NETLIST" type="int" value="24"/>
		<constant name="GLIBTOP_CMND_NETLOAD" type="int" value="22"/>
		<constant name="GLIBTOP_CMND_PPP" type="int" value="23"/>
		<constant name="GLIBTOP_CMND_PROCLIST" type="int" value="10"/>
		<constant name="GLIBTOP_CMND_PROC_AFFINITY" type="int" value="27"/>
		<constant name="GLIBTOP_CMND_PROC_ARGS" type="int" value="18"/>
		<constant name="GLIBTOP_CMND_PROC_KERNEL" type="int" value="16"/>
		<constant name="GLIBTOP_CMND_PROC_MAP" type="int" value="19"/>
		<constant name="GLIBTOP_CMND_PROC_MEM" type="int" value="13"/>
		<constant name="GLIBTOP_CMND_PROC_OPEN_FILES" type="int" value="25"/>
		<constant name="GLIBTOP_CMND_PROC_SEGMENT" type="int" value="17"/>
		<constant name="GLIBTOP_CMND_PROC_SIGNAL" type="int" value="15"/>
		<constant name="GLIBTOP_CMND_PROC_STATE" type="int" value="11"/>
		<constant name="GLIBTOP_CMND_PROC_TIME" type="int" value="14"/>
		<constant name="GLIBTOP_CMND_PROC_UID" type="int" value="12"/>
		<constant name="GLIBTOP_CMND_PROC_WD" type="int" value="26"/>
		<constant name="GLIBTOP_CMND_QUIT" type="int" value="0"/>
		<constant name="GLIBTOP_CMND_SEM_LIMITS" type="int" value="9"/>
		<constant name="GLIBTOP_CMND_SHM_LIMITS" type="int" value="7"/>
		<constant name="GLIBTOP_CMND_SWAP" type="int" value="4"/>
		<constant name="GLIBTOP_CMND_SYSDEPS" type="int" value="1"/>
		<constant name="GLIBTOP_CMND_UPTIME" type="int" value="5"/>
		<constant name="GLIBTOP_CPU_FREQUENCY" type="int" value="5"/>
		<constant name="GLIBTOP_CPU_IDLE" type="int" value="4"/>
		<constant name="GLIBTOP_CPU_IOWAIT" type="int" value="12"/>
		<constant name="GLIBTOP_CPU_IRQ" type="int" value="13"/>
		<constant name="GLIBTOP_CPU_NICE" type="int" value="2"/>
		<constant name="GLIBTOP_CPU_SOFTIRQ" type="int" value="14"/>
		<constant name="GLIBTOP_CPU_SYS" type="int" value="3"/>
		<constant name="GLIBTOP_CPU_TOTAL" type="int" value="0"/>
		<constant name="GLIBTOP_CPU_USER" type="int" value="1"/>
		<constant name="GLIBTOP_ERROR_METHOD_ABORT" type="int" value="3"/>
		<constant name="GLIBTOP_ERROR_METHOD_IGNORE" type="int" value="0"/>
		<constant name="GLIBTOP_ERROR_METHOD_WARN" type="int" value="2"/>
		<constant name="GLIBTOP_ERROR_METHOD_WARN_ONCE" type="int" value="1"/>
		<constant name="GLIBTOP_EXCLUDE_IDLE" type="int" value="4096"/>
		<constant name="GLIBTOP_EXCLUDE_NOTTY" type="int" value="16384"/>
		<constant name="GLIBTOP_EXCLUDE_SYSTEM" type="int" value="8192"/>
		<constant name="GLIBTOP_FEATURES_EXCEPT" type="int" value="8"/>
		<constant name="GLIBTOP_FEATURES_NO_SERVER" type="int" value="4"/>
		<constant name="GLIBTOP_FILE_ENTRY_FD" type="int" value="0"/>
		<constant name="GLIBTOP_FILE_ENTRY_INETSOCKET_DST_HOST" type="int" value="3"/>
		<constant name="GLIBTOP_FILE_ENTRY_INETSOCKET_DST_PORT" type="int" value="4"/>
		<constant name="GLIBTOP_FILE_ENTRY_NAME" type="int" value="1"/>
		<constant name="GLIBTOP_FILE_ENTRY_TYPE" type="int" value="2"/>
		<constant name="GLIBTOP_FSUSAGE_BAVAIL" type="int" value="2"/>
		<constant name="GLIBTOP_FSUSAGE_BFREE" type="int" value="1"/>
		<constant name="GLIBTOP_FSUSAGE_BLOCKS" type="int" value="0"/>
		<constant name="GLIBTOP_FSUSAGE_BLOCK_SIZE" type="int" value="5"/>
		<constant name="GLIBTOP_FSUSAGE_FFREE" type="int" value="4"/>
		<constant name="GLIBTOP_FSUSAGE_FILES" type="int" value="3"/>
		<constant name="GLIBTOP_FSUSAGE_READ" type="int" value="6"/>
		<constant name="GLIBTOP_FSUSAGE_WRITE" type="int" value="7"/>
		<constant name="GLIBTOP_INIT_NO_INIT" type="int" value="2"/>
		<constant name="GLIBTOP_INIT_NO_OPEN" type="int" value="1"/>
		<constant name="GLIBTOP_IPC_MSGMAP" type="int" value="1"/>
		<constant name="GLIBTOP_IPC_MSGMAX" type="int" value="2"/>
		<constant name="GLIBTOP_IPC_MSGMNB" type="int" value="3"/>
		<constant name="GLIBTOP_IPC_MSGMNI" type="int" value="4"/>
		<constant name="GLIBTOP_IPC_MSGPOOL" type="int" value="0"/>
		<constant name="GLIBTOP_IPC_MSGSSZ" type="int" value="5"/>
		<constant name="GLIBTOP_IPC_MSGTQL" type="int" value="6"/>
		<constant name="GLIBTOP_IPC_SEMAEM" type="int" value="9"/>
		<constant name="GLIBTOP_IPC_SEMMAP" type="int" value="0"/>
		<constant name="GLIBTOP_IPC_SEMMNI" type="int" value="1"/>
		<constant name="GLIBTOP_IPC_SEMMNS" type="int" value="2"/>
		<constant name="GLIBTOP_IPC_SEMMNU" type="int" value="3"/>
		<constant name="GLIBTOP_IPC_SEMMSL" type="int" value="4"/>
		<constant name="GLIBTOP_IPC_SEMOPM" type="int" value="5"/>
		<constant name="GLIBTOP_IPC_SEMUME" type="int" value="6"/>
		<constant name="GLIBTOP_IPC_SEMUSZ" type="int" value="7"/>
		<constant name="GLIBTOP_IPC_SEMVMX" type="int" value="8"/>
		<constant name="GLIBTOP_IPC_SHMALL" type="int" value="4"/>
		<constant name="GLIBTOP_IPC_SHMMAX" type="int" value="0"/>
		<constant name="GLIBTOP_IPC_SHMMIN" type="int" value="1"/>
		<constant name="GLIBTOP_IPC_SHMMNI" type="int" value="2"/>
		<constant name="GLIBTOP_IPC_SHMSEG" type="int" value="3"/>
		<constant name="GLIBTOP_KERN_PROC_ALL" type="int" value="0"/>
		<constant name="GLIBTOP_KERN_PROC_MASK" type="int" value="15"/>
		<constant name="GLIBTOP_KERN_PROC_PGRP" type="int" value="2"/>
		<constant name="GLIBTOP_KERN_PROC_PID" type="int" value="1"/>
		<constant name="GLIBTOP_KERN_PROC_RUID" type="int" value="6"/>
		<constant name="GLIBTOP_KERN_PROC_SESSION" type="int" value="3"/>
		<constant name="GLIBTOP_KERN_PROC_TTY" type="int" value="4"/>
		<constant name="GLIBTOP_KERN_PROC_UID" type="int" value="5"/>
		<constant name="GLIBTOP_LOADAVG_LAST_PID" type="int" value="3"/>
		<constant name="GLIBTOP_LOADAVG_LOADAVG" type="int" value="0"/>
		<constant name="GLIBTOP_LOADAVG_NR_RUNNING" type="int" value="1"/>
		<constant name="GLIBTOP_LOADAVG_NR_TASKS" type="int" value="2"/>
		<constant name="GLIBTOP_MAP_ENTRY_DEVICE" type="int" value="5"/>
		<constant name="GLIBTOP_MAP_ENTRY_END" type="int" value="1"/>
		<constant name="GLIBTOP_MAP_ENTRY_FILENAME" type="int" value="6"/>
		<constant name="GLIBTOP_MAP_ENTRY_INODE" type="int" value="4"/>
		<constant name="GLIBTOP_MAP_ENTRY_OFFSET" type="int" value="2"/>
		<constant name="GLIBTOP_MAP_ENTRY_PERM" type="int" value="3"/>
		<constant name="GLIBTOP_MAP_ENTRY_PRIVATE_CLEAN" type="int" value="11"/>
		<constant name="GLIBTOP_MAP_ENTRY_PRIVATE_DIRTY" type="int" value="12"/>
		<constant name="GLIBTOP_MAP_ENTRY_RSS" type="int" value="8"/>
		<constant name="GLIBTOP_MAP_ENTRY_SHARED_CLEAN" type="int" value="9"/>
		<constant name="GLIBTOP_MAP_ENTRY_SHARED_DIRTY" type="int" value="10"/>
		<constant name="GLIBTOP_MAP_ENTRY_SIZE" type="int" value="7"/>
		<constant name="GLIBTOP_MAP_ENTRY_START" type="int" value="0"/>
		<constant name="GLIBTOP_MAP_FILENAME_LEN" type="int" value="215"/>
		<constant name="GLIBTOP_MAP_PERM_EXECUTE" type="int" value="4"/>
		<constant name="GLIBTOP_MAP_PERM_PRIVATE" type="int" value="16"/>
		<constant name="GLIBTOP_MAP_PERM_READ" type="int" value="1"/>
		<constant name="GLIBTOP_MAP_PERM_SHARED" type="int" value="8"/>
		<constant name="GLIBTOP_MAP_PERM_WRITE" type="int" value="2"/>
		<constant name="GLIBTOP_MAX_CMND" type="int" value="28"/>
		<constant name="GLIBTOP_MAX_CPU" type="int" value="18"/>
		<constant name="GLIBTOP_MAX_FSUSAGE" type="int" value="8"/>
		<constant name="GLIBTOP_MAX_GROUPS" type="int" value="64"/>
		<constant name="GLIBTOP_MAX_LOADAVG" type="int" value="4"/>
		<constant name="GLIBTOP_MAX_MAP_ENTRY" type="int" value="13"/>
		<constant name="GLIBTOP_MAX_MEM" type="int" value="8"/>
		<constant name="GLIBTOP_MAX_MOUNTLIST" type="int" value="3"/>
		<constant name="GLIBTOP_MAX_MSG_LIMITS" type="int" value="7"/>
		<constant name="GLIBTOP_MAX_NETLIST" type="int" value="1"/>
		<constant name="GLIBTOP_MAX_NETLOAD" type="int" value="18"/>
		<constant name="GLIBTOP_MAX_OPEN_FILE_ENTRY" type="int" value="5"/>
		<constant name="GLIBTOP_MAX_PPP" type="int" value="3"/>
		<constant name="GLIBTOP_MAX_PROCLIST" type="int" value="3"/>
		<constant name="GLIBTOP_MAX_PROC_ARGS" type="int" value="1"/>
		<constant name="GLIBTOP_MAX_PROC_KERNEL" type="int" value="9"/>
		<constant name="GLIBTOP_MAX_PROC_MAP" type="int" value="3"/>
		<constant name="GLIBTOP_MAX_PROC_MEM" type="int" value="6"/>
		<constant name="GLIBTOP_MAX_PROC_OPEN_FILES" type="int" value="3"/>
		<constant name="GLIBTOP_MAX_PROC_SEGMENT" type="int" value="8"/>
		<constant name="GLIBTOP_MAX_PROC_SIGNAL" type="int" value="4"/>
		<constant name="GLIBTOP_MAX_PROC_STATE" type="int" value="9"/>
		<constant name="GLIBTOP_MAX_PROC_TIME" type="int" value="11"/>
		<constant name="GLIBTOP_MAX_PROC_UID" type="int" value="18"/>
		<constant name="GLIBTOP_MAX_SEM_LIMITS" type="int" value="10"/>
		<constant name="GLIBTOP_MAX_SHM_LIMITS" type="int" value="5"/>
		<constant name="GLIBTOP_MAX_SWAP" type="int" value="5"/>
		<constant name="GLIBTOP_MAX_SYSDEPS" type="int" value="27"/>
		<constant name="GLIBTOP_MAX_SYSINFO" type="int" value="2"/>
		<constant name="GLIBTOP_MAX_UPTIME" type="int" value="3"/>
		<constant name="GLIBTOP_MEM_BUFFER" type="int" value="4"/>
		<constant name="GLIBTOP_MEM_CACHED" type="int" value="5"/>
		<constant name="GLIBTOP_MEM_FREE" type="int" value="2"/>
		<constant name="GLIBTOP_MEM_LOCKED" type="int" value="7"/>
		<constant name="GLIBTOP_MEM_SHARED" type="int" value="3"/>
		<constant name="GLIBTOP_MEM_TOTAL" type="int" value="0"/>
		<constant name="GLIBTOP_MEM_USED" type="int" value="1"/>
		<constant name="GLIBTOP_MEM_USER" type="int" value="6"/>
		<constant name="GLIBTOP_METHOD_DIRECT" type="int" value="1"/>
		<constant name="GLIBTOP_METHOD_INET" type="int" value="3"/>
		<constant name="GLIBTOP_METHOD_PIPE" type="int" value="2"/>
		<constant name="GLIBTOP_METHOD_UNIX" type="int" value="4"/>
		<constant name="GLIBTOP_MOUNTENTRY_LEN" type="int" value="79"/>
		<constant name="GLIBTOP_MOUNTLIST_NUMBER" type="int" value="0"/>
		<constant name="GLIBTOP_MOUNTLIST_SIZE" type="int" value="2"/>
		<constant name="GLIBTOP_MOUNTLIST_TOTAL" type="int" value="1"/>
		<constant name="GLIBTOP_NCPU" type="int" value="32"/>
		<constant name="GLIBTOP_NETLIST_NUMBER" type="int" value="0"/>
		<constant name="GLIBTOP_NETLOAD_ADDRESS" type="int" value="3"/>
		<constant name="GLIBTOP_NETLOAD_ADDRESS6" type="int" value="14"/>
		<constant name="GLIBTOP_NETLOAD_BYTES_IN" type="int" value="7"/>
		<constant name="GLIBTOP_NETLOAD_BYTES_OUT" type="int" value="8"/>
		<constant name="GLIBTOP_NETLOAD_BYTES_TOTAL" type="int" value="9"/>
		<constant name="GLIBTOP_NETLOAD_COLLISIONS" type="int" value="13"/>
		<constant name="GLIBTOP_NETLOAD_ERRORS_IN" type="int" value="10"/>
		<constant name="GLIBTOP_NETLOAD_ERRORS_OUT" type="int" value="11"/>
		<constant name="GLIBTOP_NETLOAD_ERRORS_TOTAL" type="int" value="12"/>
		<constant name="GLIBTOP_NETLOAD_HWADDRESS" type="int" value="17"/>
		<constant name="GLIBTOP_NETLOAD_IF_FLAGS" type="int" value="0"/>
		<constant name="GLIBTOP_NETLOAD_MTU" type="int" value="1"/>
		<constant name="GLIBTOP_NETLOAD_PACKETS_IN" type="int" value="4"/>
		<constant name="GLIBTOP_NETLOAD_PACKETS_OUT" type="int" value="5"/>
		<constant name="GLIBTOP_NETLOAD_PACKETS_TOTAL" type="int" value="6"/>
		<constant name="GLIBTOP_NETLOAD_PREFIX6" type="int" value="15"/>
		<constant name="GLIBTOP_NETLOAD_SCOPE6" type="int" value="16"/>
		<constant name="GLIBTOP_NETLOAD_SUBNET" type="int" value="2"/>
		<constant name="GLIBTOP_OPEN_DEST_HOST_LEN" type="int" value="46"/>
		<constant name="GLIBTOP_OPEN_FILENAME_LEN" type="int" value="215"/>
		<constant name="GLIBTOP_PARAM_COMMAND" type="int" value="3"/>
		<constant name="GLIBTOP_PARAM_ERROR_METHOD" type="int" value="6"/>
		<constant name="GLIBTOP_PARAM_FEATURES" type="int" value="2"/>
		<constant name="GLIBTOP_PARAM_HOST" type="int" value="4"/>
		<constant name="GLIBTOP_PARAM_METHOD" type="int" value="1"/>
		<constant name="GLIBTOP_PARAM_PORT" type="int" value="5"/>
		<constant name="GLIBTOP_PARAM_REQUIRED" type="int" value="7"/>
		<constant name="GLIBTOP_PPP_BYTES_IN" type="int" value="1"/>
		<constant name="GLIBTOP_PPP_BYTES_OUT" type="int" value="2"/>
		<constant name="GLIBTOP_PPP_STATE" type="int" value="0"/>
		<constant name="GLIBTOP_PROCESS_DEAD" type="int" value="64"/>
		<constant name="GLIBTOP_PROCESS_INTERRUPTIBLE" type="int" value="2"/>
		<constant name="GLIBTOP_PROCESS_RUNNING" type="int" value="1"/>
		<constant name="GLIBTOP_PROCESS_STOPPED" type="int" value="16"/>
		<constant name="GLIBTOP_PROCESS_SWAPPING" type="int" value="32"/>
		<constant name="GLIBTOP_PROCESS_UNINTERRUPTIBLE" type="int" value="4"/>
		<constant name="GLIBTOP_PROCESS_ZOMBIE" type="int" value="8"/>
		<constant name="GLIBTOP_PROCLIST_NUMBER" type="int" value="0"/>
		<constant name="GLIBTOP_PROCLIST_SIZE" type="int" value="2"/>
		<constant name="GLIBTOP_PROCLIST_TOTAL" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_AFFINITY_ALL" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_AFFINITY_NUMBER" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_ARGS_SIZE" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_KERNEL_CMAJ_FLT" type="int" value="4"/>
		<constant name="GLIBTOP_PROC_KERNEL_CMIN_FLT" type="int" value="3"/>
		<constant name="GLIBTOP_PROC_KERNEL_KSTK_EIP" type="int" value="6"/>
		<constant name="GLIBTOP_PROC_KERNEL_KSTK_ESP" type="int" value="5"/>
		<constant name="GLIBTOP_PROC_KERNEL_K_FLAGS" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_KERNEL_MAJ_FLT" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_KERNEL_MIN_FLT" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_KERNEL_NWCHAN" type="int" value="7"/>
		<constant name="GLIBTOP_PROC_KERNEL_WCHAN" type="int" value="8"/>
		<constant name="GLIBTOP_PROC_MAP_NUMBER" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_MAP_SIZE" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_MAP_TOTAL" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_MEM_RESIDENT" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_MEM_RSS" type="int" value="4"/>
		<constant name="GLIBTOP_PROC_MEM_RSS_RLIM" type="int" value="5"/>
		<constant name="GLIBTOP_PROC_MEM_SHARE" type="int" value="3"/>
		<constant name="GLIBTOP_PROC_MEM_SIZE" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_MEM_VSIZE" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_OPEN_FILES_NUMBER" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_OPEN_FILES_SIZE" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_OPEN_FILES_TOTAL" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_SEGMENT_DATA_RSS" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_SEGMENT_DIRTY_SIZE" type="int" value="4"/>
		<constant name="GLIBTOP_PROC_SEGMENT_END_CODE" type="int" value="6"/>
		<constant name="GLIBTOP_PROC_SEGMENT_SHLIB_RSS" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_SEGMENT_STACK_RSS" type="int" value="3"/>
		<constant name="GLIBTOP_PROC_SEGMENT_START_CODE" type="int" value="5"/>
		<constant name="GLIBTOP_PROC_SEGMENT_START_STACK" type="int" value="7"/>
		<constant name="GLIBTOP_PROC_SEGMENT_TEXT_RSS" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_SIGNAL_BLOCKED" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_SIGNAL_SIGCATCH" type="int" value="3"/>
		<constant name="GLIBTOP_PROC_SIGNAL_SIGIGNORE" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_SIGNAL_SIGNAL" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_STATE_CMD" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_STATE_GID" type="int" value="3"/>
		<constant name="GLIBTOP_PROC_STATE_HAS_CPU" type="int" value="6"/>
		<constant name="GLIBTOP_PROC_STATE_LAST_PROCESSOR" type="int" value="8"/>
		<constant name="GLIBTOP_PROC_STATE_PROCESSOR" type="int" value="7"/>
		<constant name="GLIBTOP_PROC_STATE_RGID" type="int" value="5"/>
		<constant name="GLIBTOP_PROC_STATE_RUID" type="int" value="4"/>
		<constant name="GLIBTOP_PROC_STATE_STATE" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_STATE_UID" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_TIME_CSTIME" type="int" value="5"/>
		<constant name="GLIBTOP_PROC_TIME_CUTIME" type="int" value="4"/>
		<constant name="GLIBTOP_PROC_TIME_FREQUENCY" type="int" value="8"/>
		<constant name="GLIBTOP_PROC_TIME_IT_REAL_VALUE" type="int" value="7"/>
		<constant name="GLIBTOP_PROC_TIME_RTIME" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_TIME_START_TIME" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_TIME_STIME" type="int" value="3"/>
		<constant name="GLIBTOP_PROC_TIME_TIMEOUT" type="int" value="6"/>
		<constant name="GLIBTOP_PROC_TIME_UTIME" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_TIME_XCPU_STIME" type="int" value="10"/>
		<constant name="GLIBTOP_PROC_TIME_XCPU_UTIME" type="int" value="9"/>
		<constant name="GLIBTOP_PROC_UID_EGID" type="int" value="3"/>
		<constant name="GLIBTOP_PROC_UID_EUID" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_UID_FSGID" type="int" value="7"/>
		<constant name="GLIBTOP_PROC_UID_FSUID" type="int" value="6"/>
		<constant name="GLIBTOP_PROC_UID_GID" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_UID_GROUPS" type="int" value="17"/>
		<constant name="GLIBTOP_PROC_UID_NGROUPS" type="int" value="16"/>
		<constant name="GLIBTOP_PROC_UID_NICE" type="int" value="15"/>
		<constant name="GLIBTOP_PROC_UID_PGRP" type="int" value="10"/>
		<constant name="GLIBTOP_PROC_UID_PID" type="int" value="8"/>
		<constant name="GLIBTOP_PROC_UID_PPID" type="int" value="9"/>
		<constant name="GLIBTOP_PROC_UID_PRIORITY" type="int" value="14"/>
		<constant name="GLIBTOP_PROC_UID_SESSION" type="int" value="11"/>
		<constant name="GLIBTOP_PROC_UID_SGID" type="int" value="5"/>
		<constant name="GLIBTOP_PROC_UID_SUID" type="int" value="4"/>
		<constant name="GLIBTOP_PROC_UID_TPGID" type="int" value="13"/>
		<constant name="GLIBTOP_PROC_UID_TTY" type="int" value="12"/>
		<constant name="GLIBTOP_PROC_UID_UID" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_WD_EXE" type="int" value="2"/>
		<constant name="GLIBTOP_PROC_WD_EXE_LEN" type="int" value="215"/>
		<constant name="GLIBTOP_PROC_WD_MAX" type="int" value="3"/>
		<constant name="GLIBTOP_PROC_WD_NUMBER" type="int" value="0"/>
		<constant name="GLIBTOP_PROC_WD_ROOT" type="int" value="1"/>
		<constant name="GLIBTOP_PROC_WD_ROOT_LEN" type="int" value="215"/>
		<constant name="GLIBTOP_SUID_CPU" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_LOADAVG" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_MEM" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_MSG_LIMITS" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_NETLIST" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_NETLOAD" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PPP" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROCLIST" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_AFFINITY" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_ARGS" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_FILE" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_KERNEL" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_MAP" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_MEM" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_SEGMENT" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_SIGNAL" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_STATE" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_TIME" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_UID" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_PROC_WD" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_SEM_LIMITS" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_SHM_LIMITS" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_SWAP" type="int" value="0"/>
		<constant name="GLIBTOP_SUID_UPTIME" type="int" value="0"/>
		<constant name="GLIBTOP_SWAP_FREE" type="int" value="2"/>
		<constant name="GLIBTOP_SWAP_PAGEIN" type="int" value="3"/>
		<constant name="GLIBTOP_SWAP_PAGEOUT" type="int" value="4"/>
		<constant name="GLIBTOP_SWAP_TOTAL" type="int" value="0"/>
		<constant name="GLIBTOP_SWAP_USED" type="int" value="1"/>
		<constant name="GLIBTOP_SYSDEPS_ALL" type="int" value="0"/>
		<constant name="GLIBTOP_SYSDEPS_CPU" type="int" value="1"/>
		<constant name="GLIBTOP_SYSDEPS_FEATURES" type="int" value="0"/>
		<constant name="GLIBTOP_SYSDEPS_FSUSAGE" type="int" value="20"/>
		<constant name="GLIBTOP_SYSDEPS_LOADAVG" type="int" value="5"/>
		<constant name="GLIBTOP_SYSDEPS_MEM" type="int" value="2"/>
		<constant name="GLIBTOP_SYSDEPS_MOUNTLIST" type="int" value="19"/>
		<constant name="GLIBTOP_SYSDEPS_MSG_LIMITS" type="int" value="7"/>
		<constant name="GLIBTOP_SYSDEPS_NETLIST" type="int" value="23"/>
		<constant name="GLIBTOP_SYSDEPS_NETLOAD" type="int" value="21"/>
		<constant name="GLIBTOP_SYSDEPS_PPP" type="int" value="22"/>
		<constant name="GLIBTOP_SYSDEPS_PROCLIST" type="int" value="9"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_AFFINITY" type="int" value="26"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_ARGS" type="int" value="17"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_KERNEL" type="int" value="15"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_MAP" type="int" value="18"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_MEM" type="int" value="12"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_OPEN_FILES" type="int" value="24"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_SEGMENT" type="int" value="16"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_SIGNAL" type="int" value="14"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_STATE" type="int" value="10"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_TIME" type="int" value="13"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_UID" type="int" value="11"/>
		<constant name="GLIBTOP_SYSDEPS_PROC_WD" type="int" value="25"/>
		<constant name="GLIBTOP_SYSDEPS_SEM_LIMITS" type="int" value="8"/>
		<constant name="GLIBTOP_SYSDEPS_SHM_LIMITS" type="int" value="6"/>
		<constant name="GLIBTOP_SYSDEPS_SWAP" type="int" value="3"/>
		<constant name="GLIBTOP_SYSDEPS_UPTIME" type="int" value="4"/>
		<constant name="GLIBTOP_SYSINFO_CPUINFO" type="int" value="1"/>
		<constant name="GLIBTOP_SYSINFO_NCPU" type="int" value="0"/>
		<constant name="GLIBTOP_UPTIME_BOOT_TIME" type="int" value="2"/>
		<constant name="GLIBTOP_UPTIME_IDLETIME" type="int" value="1"/>
		<constant name="GLIBTOP_UPTIME_UPTIME" type="int" value="0"/>
		<constant name="GLIBTOP_XCPU_FLAGS" type="int" value="11"/>
		<constant name="GLIBTOP_XCPU_IDLE" type="int" value="10"/>
		<constant name="GLIBTOP_XCPU_IOWAIT" type="int" value="15"/>
		<constant name="GLIBTOP_XCPU_IRQ" type="int" value="16"/>
		<constant name="GLIBTOP_XCPU_NICE" type="int" value="8"/>
		<constant name="GLIBTOP_XCPU_SOFTIRQ" type="int" value="17"/>
		<constant name="GLIBTOP_XCPU_SYS" type="int" value="9"/>
		<constant name="GLIBTOP_XCPU_TOTAL" type="int" value="6"/>
		<constant name="GLIBTOP_XCPU_USER" type="int" value="7"/>
		<constant name="HOSTNAMSZ" type="int" value="255"/>
		<constant name="LIBGTOP_VERSION_STRING" type="char*" value="Libgtop %s server version %s (%u,%u,%u,%u)."/>
		<constant name="MCOOKIE_NAME" type="char*" value="MAGIC-1"/>
		<constant name="MCOOKIE_SCREEN" type="char*" value="42980"/>
		<constant name="MCOOKIE_X_NAME" type="char*" value="MIT-MAGIC-COOKIE-1"/>
		<constant name="PATCHLEVEL" type="int" value="2"/>
		<constant name="REPLYSIZ" type="int" value="300"/>
		<constant name="TABLE_SIZE" type="int" value="101"/>
		<constant name="TRUE" type="int" value="1"/>
		<union name="glibtop_response_union">
			<field name="data" type="glibtop_union"/>
			<field name="sysdeps" type="glibtop_sysdeps"/>
		</union>
		<union name="glibtop_union">
			<field name="cpu" type="glibtop_cpu"/>
			<field name="mem" type="glibtop_mem"/>
			<field name="swap" type="glibtop_swap"/>
			<field name="uptime" type="glibtop_uptime"/>
			<field name="loadavg" type="glibtop_loadavg"/>
			<field name="shm_limits" type="glibtop_shm_limits"/>
			<field name="msg_limits" type="glibtop_msg_limits"/>
			<field name="sem_limits" type="glibtop_sem_limits"/>
			<field name="proclist" type="glibtop_proclist"/>
			<field name="proc_state" type="glibtop_proc_state"/>
			<field name="proc_uid" type="glibtop_proc_uid"/>
			<field name="proc_mem" type="glibtop_proc_mem"/>
			<field name="proc_time" type="glibtop_proc_time"/>
			<field name="proc_signal" type="glibtop_proc_signal"/>
			<field name="proc_kernel" type="glibtop_proc_kernel"/>
			<field name="proc_segment" type="glibtop_proc_segment"/>
			<field name="proc_args" type="glibtop_proc_args"/>
			<field name="proc_map" type="glibtop_proc_map"/>
			<field name="mountlist" type="glibtop_mountlist"/>
			<field name="fsusage" type="glibtop_fsusage"/>
			<field name="netlist" type="glibtop_netlist"/>
			<field name="netload" type="glibtop_netload"/>
			<field name="ppp" type="glibtop_ppp"/>
			<field name="proc_open_files" type="glibtop_proc_open_files"/>
			<field name="proc_wd" type="glibtop_proc_wd"/>
			<field name="proc_affinity" type="glibtop_proc_affinity"/>
		</union>
	</namespace>
</api>

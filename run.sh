#前台提交
bsub -b -I -q q_sw_expr -host_stack 1024 -share_size 4096 -n 64 -cgsp 64 ./benchmark-optimized 7 768 768 768 64 /home/export/online1/cpc/pre/stencil_data_7_768x768x768_64steps /home/export/online1/cpc/pre/stencil_answer_7_768x768x768_64steps  2>&1 | tee lo log.run
#后台提交
#bsub -b -o run.loh -q q_sw_expr -host_stack 1024 -share_size 4096 -n 16 -cgsp 64 ./benchmark-nativ 7 512 512 512 48 /home/export/online1/cpc/pre/stencil_data_512x512x512 /home/export/online1/cpc/pre/stencil_answer_7_512x512x512_48steps

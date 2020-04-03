# 国产CPU应用挑战赛初赛
优化思路：
1. 将单进程版源码更改为多进程版（16，64进程）使用MPI解决进程间数据依赖（初次优化使用的同步MPI）
2. 将计算部分交给从核，使用DMA技术。
3. 双缓冲技术。
3. 进行主从异步。使用异步MPI。在进程传输同时进行主从核传输。
4. 删除双缓冲。时间换空间。

[试题说明与下载](https://mp.weixin.qq.com/s?__biz=MzU5MzAzMDM4Nw==&mid=2247484186&idx=1&sn=54e962909fbe87bad6c1e4f75dd2cd2c&chksm=fe17fe86c9607790357dc670250731dc3f879742ce267ab9e93780efe8da1f45a682ead94170&scene=21#wechat_redirect)
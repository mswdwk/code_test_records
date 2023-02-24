# Split video 
ffmpeg -ss start_time_s -to end_time_s -i input_video.mp4  -c copy output_video.mp4

# Merge video
ffmpeg -f concat -safe 0 -i videofilelist.txt -c copy -y merge_video.mp4

# Reference
[FFmpeg 裁剪视频的几种方式](https://zhuanlan.zhihu.com/p/552259094)

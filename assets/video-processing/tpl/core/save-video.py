from moviepy.video.io.VideoFileClip import VideoFileClip

# Save video to disk.
node_in_video: VideoFileClip = in_video
node_in_path = 'in_path'
node_in_video.write_videofile(node_in_path)

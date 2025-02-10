from moviepy.video.io.VideoFileClip import VideoFileClip

# Extract audio from video.
node_in_video: VideoFileClip = in_video
node_out_audio = node_in_video.audio
node_out_video = node_in_video.without_audio()

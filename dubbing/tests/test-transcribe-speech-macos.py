import whisper
import json

# Transcribe the audio file.
n066_in_path = '/Users/dong/Documents/github/visParaflowAssets/video/res/speech.wav'
n066_out_path = 'out_path'
model = whisper.load_model("base")  # You can also try "small", "medium", or "large" for better accuracy
result = model.transcribe(n066_in_path, language="en")
transcripts = []
for segment in result["segments"]:
    start = segment["start"]  # Start time of the segment
    end = segment["end"]      # End time of the segment
    text = segment["text"]    # Transcribed text
    transcripts.append({"start": start, "end": end, "text": text})
with open(n066_out_path, "w") as f:
    json.dump(transcripts, f, indent=2)

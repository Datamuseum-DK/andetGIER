import sys, wave, struct, random

def load_wav(path, seed=1):
   random.seed(seed)
   bits = ""
   with wave.open(path, "rb") as w:
      assert w.getnchannels() == 1
      assert w.getsampwidth() == 2
      rms = 0
      while True:
         b = w.readframes(1)
         if len(b)==0: break
         f = struct.unpack("h", b)[0] / 37678.0
         rf = (f*f)**.5
         rms += (rf-rms)*.001
         f += (random.random() - .5) * rms # dithering
         bits += "01"[f>.1]
   N=40
   words = []
   while len(bits)>N:
      chunk, bits = bits[0:N], bits[N:]
      words.append(int(chunk,2))

   assert len(words)<950, ("cannot load %s: program too big" % path)
   return words

if len(sys.argv) != 2:
   sys.stderr.write("Usage: %s <pcm.wav> > pcm.asc\n" % sys.argv[0])
   sys.exit(1)

words = load_wav(sys.argv[1])

print("""
_b i=41, a3
a0:
   ; pan a1 t+a2
    pan a1 t+84 ; XXX hack, slip.py doesn't support t+<label> yet
a1:
   arn t+1 IPA
""")
for i in range(39): print("   tk 0, tk 1")
print("""
   hv a1 NPA
   hv a0
""")

print("_m")
print("a2:")
for i,w in enumerate(words):
   print(str(w) + ["","a"][i == len(words)-1])
print("_e41")

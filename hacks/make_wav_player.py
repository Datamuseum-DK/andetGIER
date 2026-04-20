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

   assert len(words)<900, ("cannot load %s: program too big" % path)
   return words

if len(sys.argv) != 3:
   sys.stderr.write("Usage: %s <sample rate: 12500 or 25000> <pcm.wav> > pcm.asc\n" % sys.argv[0])
   sys.stderr.write(".wav file must be mono and 16-bit\n")
   sys.exit(1)


rate = int(sys.argv[1])

words = load_wav(sys.argv[2])

if rate == 12500:
   print("""
_b i=41, a3
a0:
   ; pan a1 ta2-1
   pan a1 t+83 ; XXX hack, slip.py doesn't support t+<label> yet
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

elif rate == 25000:
   print("""
   _b i=41, a3
   a0:
      ; pan a1 ta2-1
       pan a1 t+63 ; XXX hack, slip.py doesn't support t+<label> yet
   a1:
      arn t+1 IPA
   """)
   for i in range(38//2): print("   tk 1, tk 1")
   print("""
      tk 1
      hv a1 NPA
      hv a0
   """)

   print("_m")
   print("a2:")
   for i,w in enumerate(words):
      print(str(w) + ["","a"][i == len(words)-1])
   print("_e41")

else:
   raise RuntimeError("unsupported sample rate %d" % rate)

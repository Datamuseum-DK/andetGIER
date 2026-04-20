import sys, wave, struct, random

def load_wav(path):
   samples = []
   with wave.open(path, "rb") as w:
      assert w.getnchannels() == 1
      assert w.getsampwidth() == 2
      while True:
         b = w.readframes(1)
         if len(b)==0: break
         f = struct.unpack("h", b)[0] / 32768.0
         samples.append(f)
   return samples

def load_wav_words(path, dither_path=None, seed=1):

   dither = None
   if dither_path is not None:
      dither = load_wav(dither_path)

   audio = load_wav(path)

   random.seed(seed)
   bits = ""
   rms = 0
   for i,f in enumerate(audio):
      rf = (f*f)**.5
      rms += (rf-rms)*.001
      noise = 0
      if dither is not None:
         noise = dither[i % len(dither)]
      else:
         noise = (random.random() - .5) * 2
      f += noise * rms # dither hack
      bias = .1
      bits += "01"[f>bias]
   N=40
   words = []
   while len(bits)>N:
      chunk, bits = bits[0:N], bits[N:]
      words.append(int(chunk,2))

   return words

if len(sys.argv) not in [4,5]:
   sys.stderr.write("Usage: %s <sample rate: 6250, 12500 or 25000> <pcm.wav> <output.asc> [dither.wav]\n" % sys.argv[0])
   sys.stderr.write(".wav file must be mono and 16-bit\n")
   sys.exit(1)

rate = int(sys.argv[1])

prg = ""

def writeline(str):
   global prg
   prg += (str + "\n")

dither_path = None
if len(sys.argv)==5: dither_path = sys.argv[4]
pcm_words = load_wav_words(sys.argv[2], dither_path)

pc0 = 41
pc = pc0

writeline("_b i=%PC0, a6")
writeline("a0:")
writeline("   pan a1 t+%TBL0")
writeline("a1:")
writeline("   arn t+1 IRA")
writeline("   ga a2")
writeline("   tk 10 , ga a4")

writeline("a2:")
writeline("   arn t+1 IQA")
pc += 5

irate=None
if rate == 6250:
   irate = 4
   for i in range(39):
      writeline("   tk 0, tk 0")
      writeline("   tk 0, tk 1")
      pc += 2
   writeline("   tk 0, tk 0")
   pc += 1
elif rate == 12500:
   irate = 2
   for i in range(39):
      writeline("   tk 0, tk 1")
      pc += 1
elif rate == 25000:
   irate = 1
   for i in range(38//2):
      writeline("   tk 1, tk 1")
      pc += 1
   writeline("   tk 1")
   pc += 1
else:
   raise RuntimeError("unsupported sample rate %d" % rate)

writeline("   hv a2 NQA")
pc += 1

writeline("a3:")
num_wait_samples = 80
for i in range(num_wait_samples//2):
   writeline("   tk 0, tk 0")
   pc += 1

writeline("a4:")
writeline("   bt t-1")
writeline("   hv a3")
writeline("   hv a1 NRA")
writeline("   hv a0")
pc += 4

writeline("a5:")
writeline("_m")

prg = prg.replace("%PC0", str(pc0))
prg = prg.replace("%TBL0", str(pc-1))

# move leading zeros to end, so we can have [pcm,wait],[pcm,wait]...
num_leading_zeros = 0
for w in pcm_words:
   if w != 0:
      break
   num_leading_zeros += 1
if num_leading_zeros > 0:
   pcm_words = pcm_words[num_leading_zeros:] + ([0]*num_leading_zeros)

zc = 0
zthres = 2
pcms = [[[],0]]
for i,w in enumerate(pcm_words):
   if w > 0:
      if zc >= zthres:
         pcms.append([[],0])
      zc = 0
   else:
      zc += 1
      if zc >= zthres:
         pcms[-1][1] = zc
   pcms[-1][0].append(w)

# remove trailing zeroes per chunk
for p in pcms:
   if p[1] > 0: p[0] = p[0][0:-p[1]]

writeline("")
writeline("; pcm ptr / wait table")
cursor = pc + len(pcms)
for i,p in enumerate(pcms):
   is_last = (i==len(pcms)-1)
   num_pcm = len(p[0])
   num_zero = p[1]
   if num_zero < 1: num_zero = 1
   num_wait = round((num_zero * 40) / num_wait_samples)*irate
   if num_wait < 1: num_wait = 1
   assert (cursor < 1024)
   assert (num_wait < 1024)
   enc = ((cursor-1) << 30) + (num_wait << 20)
   cursor += num_pcm
   writeline("%d%s" % (enc, ["","a"][is_last]))

writeline("")
writeline("; pcm tables")
for p in pcms:
   for i,w in enumerate(p[0]):
      is_last = (i==len(p[0])-1)
      writeline("%d%s" % (w, ["","a"][is_last]))

writeline("_e%d" % pc0)

with open(sys.argv[3], "w") as f:
   f.write(prg)

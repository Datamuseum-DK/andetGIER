import sys, wave, struct, random, argparse

def load_wav(path):
   with wave.open(path, "rb") as w:
      assert w.getnchannels() == 1
      assert w.getsampwidth() == 2
      samples = []
      while True:
         b = w.readframes(1)
         if len(b)==0: break
         f = struct.unpack("h", b)[0] / 32768.0
         samples.append(f)
      return samples, w.getframerate()

def load_wav_words(path, dither_path=None, seed=1):

   dither = None
   if dither_path is not None:
      dither, _ = load_wav(dither_path)

   audio, sample_rate = load_wav(path)

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

   return words, sample_rate


gier_clock = 460850
loop_uops = 19 + 13 # arn + hv
left_mode1_uops = 8
right_mode1_uops = 10
whole_mode1_uops = 10
qq_mode2_uops = 1
tk_mode2_uops = lambda n: 5+n
mode4_uops = 1


class Rate:
   def __init__(self, id):
      self.id = id
      self.lines = []
      self.uops = 0
      self.pc = 0

   def add_line(self, line):
      self.lines.append(line)

   def add_uops(self, n):
      self.uops += n

   def add_pc(self, n):
      self.pc += n

   def get_sample_rate(self):
      return gier_clock / ((loop_uops + self.uops) / 40)

   def get_num_words(self):
      return 2+self.pc

rates = []
rate = None

def r_begin(id):
   global rates, rate
   assert rate is None
   rate = Rate(id)
   rates.append(rate)

def r_line(line):
   rate.add_line(line)

def r_uops(n):
   rate.add_uops(n)

def r_pc(n):
   rate.add_pc(n)

def r_end():
   global rate
   rate = None


def define_rates():
   r_begin("U1")
   for i in range(38//2):
      r_line("   tk 1, tk 1")
      r_uops(left_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_uops(right_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_pc(1)
   r_line("   tk 1")
   r_uops(whole_mode1_uops + tk_mode2_uops(1) + mode4_uops)
   r_pc(1)
   r_end()

   r_begin("U20")
   for i in range(39):
      r_line("   qq 0, tk 1")
      r_uops(left_mode1_uops + qq_mode2_uops + mode4_uops)
      r_uops(right_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_pc(1)
   r_end()

   r_begin("U21")
   for i in range(39):
      r_line("   tk 0, tk 1")
      r_uops(left_mode1_uops + tk_mode2_uops(0) + mode4_uops)
      r_uops(right_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_pc(1)
   r_end()

   r_begin("U30")
   for i in range(39//4):
      r_line("   qq 0, tk 1")
      r_uops(left_mode1_uops + qq_mode2_uops + mode4_uops)
      r_uops(right_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_line("   tk 1, qq 0")
      r_uops(left_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_uops(right_mode1_uops + qq_mode2_uops + mode4_uops)
      r_line("   tk 1, tk 1")
      r_uops(left_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_uops(right_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_pc(3)
   r_end()

   r_begin("U31")
   for i in range(39//2):
      r_line("   qq 0, tk 1")
      r_uops(left_mode1_uops + qq_mode2_uops + mode4_uops)
      r_uops(right_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_line("   qq 0, qq 0")
      r_uops(left_mode1_uops + qq_mode2_uops + mode4_uops)
      r_uops(right_mode1_uops + qq_mode2_uops + mode4_uops)
      r_line("   tk 1, qq 0")
      r_uops(left_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_uops(right_mode1_uops + qq_mode2_uops + mode4_uops)
      r_pc(3)
   r_line("   qq 0, tk 1")
   r_uops(left_mode1_uops + qq_mode2_uops + mode4_uops)
   r_uops(right_mode1_uops + tk_mode2_uops(1) + mode4_uops)
   r_pc(1)
   r_end()

   r_begin("U40")
   for i in range(39):
      r_line("   qq 0, qq 0")
      r_uops(left_mode1_uops + qq_mode2_uops + mode4_uops)
      r_uops(right_mode1_uops + qq_mode2_uops + mode4_uops)
      r_line("   tk 1, qq 0")
      r_uops(left_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_uops(right_mode1_uops + qq_mode2_uops + mode4_uops)
      r_pc(2)
   r_end()

   r_begin("U41")
   for i in range(39):
      r_line("   qq 0, qq 0")
      r_uops(left_mode1_uops + qq_mode2_uops + mode4_uops)
      r_uops(right_mode1_uops + qq_mode2_uops + mode4_uops)
      r_line("   tk 1, tk 0")
      r_uops(left_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_uops(right_mode1_uops + tk_mode2_uops(0) + mode4_uops)
      r_pc(2)
   r_end()

   # XXX same rate as L2, but code is larger
   # r_begin("U6")
   # for i in range(39):
   #    r_line("   qq 0, tk 0")
   #    r_uops(left_mode1_uops + qq_mode2_uops + mode4_uops)
   #    r_uops(right_mode1_uops + tk_mode2_uops(0) + mode4_uops)
   #    r_line("   tk 1, tk 0")
   #    r_uops(left_mode1_uops + tk_mode2_uops(1) + mode4_uops)
   #    r_uops(right_mode1_uops + tk_mode2_uops(0) + mode4_uops)
   #    r_pc(2)
   # r_end()

   r_begin("U42")
   for i in range(39):
      r_line("   tk 0, tk 0")
      r_uops(left_mode1_uops + tk_mode2_uops(0) + mode4_uops)
      r_uops(right_mode1_uops + tk_mode2_uops(0) + mode4_uops)
      r_line("   tk 1, tk 0")
      r_uops(left_mode1_uops + tk_mode2_uops(1) + mode4_uops)
      r_uops(right_mode1_uops + tk_mode2_uops(0) + mode4_uops)
      r_pc(2)
   r_end()

   r_begin("L1")
   r_line("   pa a6 t+39")
   r_line("a6:")
   r_line("   bt t-1")
   r_line("   tk 1, hv a6")
   r_uops(13+(17+15+15)*39)
   r_pc(3)
   r_end()

   r_begin("L2")
   r_line("   pa a7 t+38")
   r_line("a6:tk 1, qq 0")
   r_line("a7:bt t-1")
   r_line("   hv a6")
   r_uops(13+(15+12+17)*39+13*38)
   r_pc(4)
   r_end()

   r_begin("L3")
   r_line("   pa a7 t+38")
   r_line("a6:tk 1, qq 0")
   r_line("a7:bt t-1")
   r_line("   qq 0, hv a6")
   r_uops(13+(15+12+17)*39+(10+15)*38)
   r_pc(4)
   r_end()

   r_begin("L4")
   r_line("   pa a7 t+38")
   r_line("a6:tk 1, tk 0")
   r_line("a7:bt t-1")
   r_line("   qq 0, hv a6")
   r_uops(13+(15+16+17)*39+(10+15)*38)
   r_pc(4)
   r_end()

   r_begin("L5")
   r_line("   pa a7 t+38")
   r_line("a6:tk 1, tk 0")
   r_line("a7:bt t-1")
   r_line("   tk 0, hv a6")
   r_uops(13+(15+16+17)*39+(14+15)*38)
   r_pc(4)
   r_end()

define_rates()
epilog=""

epilog+="All .wav inputs must be 16-bit, mono, no fancy encoding.\n"

epilog+="\n"

epilog+="The player code will always choose one of the following rates.\n"
epilog+="No resampling is done (try sox).\n"
epilog+="Unless overridden by -r/-R, the best matching sample rate from the input file is chosen\n"
for rate in rates:
   epilog += ("  %s   %.0f Hz (code size: %d words)\n" % (rate.id, rate.get_sample_rate(), rate.get_num_words()))

epilog+="\n"

epilog+="The dither .wav is used as random source (sample rate ignored).\n"
epilog+="Try some less harsh noise like:\n"
epilog+="  sox -r$RATE -c1 -b16 -n dither.wav synth 1 tpdfnoise bandreject 1000 700\n"

args = argparse.ArgumentParser(prog=sys.argv[0], epilog=epilog, formatter_class=argparse.RawTextHelpFormatter)
args.add_argument("WAVE", help="input WAVE file")
args.add_argument("-o", "--output", nargs=1, help="output GIER/SLIP assembly file")
args.add_argument("-r", "--sample-rate", nargs=1, type=float, help="override .wav sample rate, pick best matching rate id")
args.add_argument("-R", "--rate-id", nargs=1, help="override .wav sample rate using rate id, see below")
args.add_argument("-d", "--dither", nargs=1, help="dither noise WAVE file")
args = args.parse_args()


prg = ""
def writeline(str):
   global prg
   prg += (str + "\n")

pcm_words, original_sample_rate = load_wav_words(args.WAVE, args.dither and args.dither[0] or None)
rate = None
if args.rate_id is not None:
   assert args.sample_rate is None, "cannot specify both sample rate and rate id"
   for r in rates:
      if r.id == args.rate_id[0]:
         rate = r
         break
   if not rate:
      raise RuntimeError("invalid rate id [%s]" % args.rate_id[0])
else:
   sample_rate = original_sample_rate
   if args.sample_rate is not None:
      sample_rate = args.sample_rate
   best = None
   for r in rates:
      if (best is None) or (abs(r.get_sample_rate()-sample_rate) < abs(best.get_sample_rate()-sample_rate)):
         best = r
   rate = best

assert rate is not None

pc0 = 41
pc = pc0

writeline("_b i=%PC0, a9")
writeline("a0:")
writeline("   pan a1 t+%TBL0")
writeline("a1:")
writeline("   arn t+1 IRA")
writeline("   ga a2")
writeline("   tk 10 , ga a5")

writeline("a2:")
writeline("   arn t+1 IQA")
pc += 5

writeline("   ; rate code '%s' begin" % rate.id)
for line in rate.lines:
   writeline(line)
pc += rate.pc
writeline("   ; rate code '%s' end" % rate.id)

writeline("   hv a2 NQA")
pc += 1

writeline("a3: ; wait code")
writeline("   pan a4 t+99")
writeline("a4:")
writeline("   bt t-1")
writeline("   hv a4")
writeline("a5:")
writeline("   bt t-1")
writeline("   hv a3 ; wait code end")
pc += 5
uops_per_wait_iteration = 30*100
time_per_wait_iteration = uops_per_wait_iteration / gier_clock

writeline("   hv a1 NRA")
writeline("   hv a0")
pc += 2

writeline("a8:")
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
   wait_time = (num_zero * 40) / original_sample_rate
   num_wait = round(wait_time / time_per_wait_iteration)
   if num_wait < 0: num_wait = 0
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

if args.output is not None and args.output != "-":
   with open(args.output[0], "w") as f:
      f.write(prg)
else:
   print(prg)

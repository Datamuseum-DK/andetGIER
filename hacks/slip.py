#!/usr/bin/env python3

LC=(1<<0)
UC=(1<<1)
SET_LOWER = 58
SET_UPPER = 60
CR=64

GIER_FLAG_CHARS = [
( 0  , LC|UC , " "),
( 1  , LC    , "1"),
( 1  , UC    , "∨£"),
( 2  , LC    , "2"),
( 2  , UC    , "×*"),
( 3  , LC    , "3"),
( 3  , UC    , "/"),
( 4  , LC    , "4"),
( 4  , UC    , "="),
( 5  , LC    , "5"),
( 5  , UC    , ";"),
( 6  , LC    , "6"),
( 6  , UC    , "["),
( 7  , LC    , "7"),
( 7  , UC    , "]"),
( 8  , LC    , "8"),
( 8  , UC    , "("),
( 9  , LC    , "9"),
( 9  , UC    , ")"),
( 13 , LC    , "å"),
( 13 , UC    , "Å"),
( 14 , LC    , "_"),
( 14 , UC    , "|"),
( 16 , LC    , "0"),
( 16 , UC    , "∧&"),
( 17 , LC    , "<"),
( 17 , UC    , ">"),
( 18 , LC    , "s"),
( 18 , UC    , "S"),
( 19 , LC    , "t"),
( 19 , UC    , "T"),
( 20 , LC    , "u"),
( 20 , UC    , "U"),
( 21 , LC    , "v"),
( 21 , UC    , "V"),
( 22 , LC    , "w"),
( 22 , UC    , "W"),
( 23 , LC    , "x"),
( 23 , UC    , "X"),
( 24 , LC    , "y"),
( 24 , UC    , "Y"),
( 25 , LC    , "z"),
( 25 , UC    , "Z"),
( 27 , LC    , ","),
( 27 , UC    , "⏨'"),
( 32 , LC    , "-"),
( 32 , UC    , "+"),
( 33 , LC    , "j"),
( 33 , UC    , "J"),
( 34 , LC    , "k"),
( 34 , UC    , "K"),
( 35 , LC    , "l"),
( 35 , UC    , "L"),
( 36 , LC    , "m"),
( 36 , UC    , "M"),
( 37 , LC    , "n"),
( 37 , UC    , "N"),
( 38 , LC    , "o"),
( 38 , UC    , "O"),
( 39 , LC    , "p"),
( 39 , UC    , "P"),
( 40 , LC    , "q"),
( 40 , UC    , "Q"),
( 41 , LC    , "r"),
( 41 , UC    , "R"),
( 43 , LC    , "ø"),
( 43 , UC    , "Ø"),
( 48 , LC    , "æ"),
( 48 , UC    , "Æ"),
( 49 , LC    , "a"),
( 49 , UC    , "A"),
( 50 , LC    , "b"),
( 50 , UC    , "B"),
( 51 , LC    , "c"),
( 51 , UC    , "C"),
( 52 , LC    , "d"),
( 52 , UC    , "D"),
( 53 , LC    , "e"),
( 53 , UC    , "E"),
( 54 , LC    , "f"),
( 54 , UC    , "F"),
( 55 , LC    , "g"),
( 55 , UC    , "G"),
( 56 , LC    , "h"),
( 56 , UC    , "H"),
( 57 , LC    , "i"),
( 57 , UC    , "I"),
( 59 , LC    , "."),
( 59 , UC    , ":"),
( CR , LC|UC , "\n")
]

char_to_gier = {}
gier_to_lower_char = {}
gier_to_upper_char = {}
lower_chars = set()
upper_chars = set()

for gier,flag,chars in GIER_FLAG_CHARS:
   for char in chars:
      char_to_gier[char] = gier
      if flag & LC:
         lower_chars.add(char)
         gier_to_lower_char[gier] = char
      if flag & UC:
         upper_chars.add(char)
         gier_to_upper_char[gier] = char

def str2gier(str):
   is_upper = False
   gier = b""
   for char in str:
      if char not in char_to_gier:
         raise RuntimeError("[%s] cannot be GIER encoded" % char)
      code = char_to_gier[char]
      lc = (char in lower_chars)
      uc = (char in upper_chars)
      if lc and uc:
         pass
      elif uc and not is_upper:
         assert not lc
         gier += bytes([SET_UPPER])
         is_upper = True
      elif lc and is_upper:
         assert not uc
         gier += bytes([SET_LOWER])
         is_upper = False
      gier += bytes([code])
   return gier

def gier2str(gier):
   str = ""
   is_upper = False
   for b in gier:
      if b == SET_UPPER:
         is_upper = True
      elif b == SET_LOWER:
         is_upper = False
      elif is_upper:
         str += gier_to_upper_char[b]
      else:
         str += gier_to_lower_char[b]
   return str

LOWER_CR = bytes([SET_LOWER,CR])
BININ_IMAGE = str2gier("binin,image,<")
SPACE_AA = str2gier(" "*100+"å")

BIN_HEADERS = [
   LOWER_CR+BININ_IMAGE+SPACE_AA,
   BININ_IMAGE+SPACE_AA,
   SPACE_AA,
]

BITS_ADDR = [(0,9),(10,19)]
BITS_INCR = (10,19)
BITS_OP = [(20,25), (30,35)]
BIT_S = [26,36] # aka "n" (clear R)
BIT_INDIRECT = [27,37] # aka "()"
BITS_RSP = [(28,29), (38,39)]
BIT_X = 30
BIT_V = 31
BIT_D = 32
BITS_IOP = (33,34)
BITS_KZOTPQR = (35,37)
BITS_ABC = (38,39)
BIT_HALFWORD = 40
BIT_FLOAT = 41

RSP_ABSOLUTE = 0
RSP_RELATIVE = 2
RSP_INDEXED = 3
RSP_SEQUENCED = 1

IOPS = ("IMNL", "KZOTPQR", "BAC")

def parity_ok(o):
   return (bin(o)[2:].count("1") % 2)==1

def read_flx(path):
   data=[]
   with open(path, "rb") as f:
      while True:
         s = f.read(1)
         if len(s) == 0: break
         o = s[0]
         assert parity_ok(o)
         data.append((o&(1+2+4+8)) + ((o>>1)&(16+32+64)))
   return bytes(data)

def write_flx(path, bs):
   with open(path,"wb") as f:
      for b in bs:
         assert (0<=b) and (b<=127)
         o = (b&(1+2+4+8)) + ((b&(16+32+64))<<1);
         if not parity_ok(o): o += 16
         assert parity_ok(o)
         f.write(bytes([o]))

OPNUM2NAM = {
   # page 160 in https://datamuseum.dk/w/images/b/be/LaerebogI.pdf
   0:  "QQ", 1:  "ZQ", 2:  "AR", 3:  "SR", 4:  "AN", 5:  "SN", 6:  "AC", 7:  "SC",
   8:  "MB", 9:  "AB", 10: "MT", 11: "MK", 12: "ML", 13: "DK", 14: "DL", 15: "NK",
   16: "NL", 17: "HR", 18: "TL", 19: "CK", 20: "CL", 21: "GR", 22: "GA", 23: "GT",
   24: "TK", 25: "CA", 26: "GM", 27: "PM", 28: "XR", 29: "GI", 30: "PS", 31: "PP",
   32: "PA", 33: "PT", 34: "HK", 35: "PI", 36: "IS", 37: "IT", 38: "CM", 39: "BT",
   40: "NS", 41: "NT", 42: "GP", 43: "NC", 44: None, 45: None, 46: None, 47: None,
   48: None, 49: "BS", 50: "HS", 51: "VY", 52: "LK", 53: "SK", 54: "GK", 55: "VK",
   56: "HV", 57: None, 58: "SY", 59: "LY", 60: "HH", 61: "GS", 62: None, 63: "UD",
}

OPNAM2NUM = dict([(opnam,opnum) for (opnum,opnam) in OPNUM2NAM.items() if opnam is not None])

def _binf(a,b):
   a=41-a
   b=41-b
   a,b=b,a
   assert a<=b, ("%d<=%d" % (a,b))
   n = (b-a+1)
   return (a,b,n)

def get_bits(v,a,b):
   a,b,n = _binf(a,b)
   return (v>>a) & ((1<<n)-1)

def set_bits(v,a,b,vv):
   a,b,n = _binf(a,b)
   mask = ((1<<n)-1) << a
   return (v & (~mask)) | ((vv<<a)&mask)

def signed10(v):
   if v > 511: v = -1024+v
   if v == 0: return ""
   if v > 0: return ("+%d"%v)
   if v < 0: return ("%d"%v)
   assert False, "unreachable"

class Op:
   def __init__(self, word=0):
      self.word = word
      self.unresolved_addr_name = [None,None]
      self.pc = None

   def set_pc(self, pc): self.pc = pc

   def get_bits(self, a, b): return get_bits(self.word, a, b)
   def get_bit(self, a): return self.get_bits(a,a)
   def set_bits(self, a, b, v): self.word = set_bits(self.word, a, b, v)
   def set_bit(self, a, v): self.set_bits(a,a,[0,1][bool(v)])

   def is_fullword(self): return self.get_bit(BIT_HALFWORD) == 0
   def is_halfword(self): return not self.is_fullword()

   def make_fullword(self): self.set_bit(BIT_HALFWORD,0)
   def make_halfword(self): self.set_bit(BIT_HALFWORD,1)

   def get_float(self): return self.get_bit(BIT_FLOAT) == 1
   def set_float(self,v): self.set_bit(BIT_FLOAT,v)

   def check_half(self, half):
      if half == 1: assert self.is_halfword()
      assert half==0 or half==1

   def get_addr(self, half):
      self.check_half(half)
      return self.get_bits(*BITS_ADDR[half])

   def set_addr(self, half, v):
      self.check_half(half)
      return self.set_bits(*BITS_ADDR[half],v)

   def set_unresolved_addr_name(self, half, name):
      self.check_half(half)
      self.unresolved_addr_name[half] = name

   def get_unresolved_addr_names(self):
      return set([x for x in self.unresolved_addr_name if x is not None])

   def resolve_relative(self, name, addr_pc):
      assert self.pc is not None, "op.set_pc() not called?"
      r = (addr_pc - self.pc)
      did_resolve = False
      for half in range(2):
         if name != self.unresolved_addr_name[half]:
            continue
         self.set_rsp(half, RSP_RELATIVE)
         self.set_addr(half, r)
         self.unresolved_addr_name[half] = None
         did_resolve = True
      assert did_resolve

   def resolve_absolute(self, name, addr_pc):
      for half in range(2):
         if name != self.unresolved_addr_name[half]:
            continue
         self.set_rsp(half, RSP_ABSOLUTE)
         self.set_addr(half, addr_pc)
         self.unresolved_addr_name[half] = None
         did_resolve = True
      assert did_resolve

   def get_opnum(self, half):
      self.check_half(half)
      return self.get_bits(*BITS_OP[half])

   def set_opnum(self, half, v):
      self.check_half(half)
      return self.set_bits(*BITS_OP[half], v)

   def get_incr(self):
      assert self.is_fullword()
      return self.get_bits(*BITS_INCR)

   def set_incr(self, v):
      assert self.is_fullword()
      return self.set_bits(*BITS_INCR, v)

   def get_rsp(self, half):
      self.check_half(half)
      return self.get_bits(*BITS_RSP[half])

   def set_rsp(self, half, v):
      self.check_half(half)
      return self.set_bits(*BITS_RSP[half], v)

   def get_indirect(self, half):
      self.check_half(half)
      return self.get_bit(BIT_INDIRECT[half]) == 1

   def set_indirect(self, half, v):
      self.check_half(half)
      self.set_bit(BIT_INDIRECT[half], v)

   def get_S(self, half):
      self.check_half(half)
      return self.get_bit(BIT_S[half]) == 1

   def set_S(self, half, v):
      self.check_half(half)
      self.set_bit(BIT_S[half], v)

   def get_X(self):
      assert self.is_fullword()
      return self.get_bit(BIT_X) == 1

   def set_X(self,v):
      assert self.is_fullword()
      self.set_bit(BIT_X,v)

   def get_V(self):
      assert self.is_fullword()
      return self.get_bit(BIT_V) == 1

   def set_V(self,v):
      assert self.is_fullword()
      self.set_bit(BIT_V,v)

   def get_D(self):
      assert self.is_fullword()
      return self.get_bit(BIT_D) == 1

   def set_D(self,v):
      assert self.is_fullword()
      self.set_bit(BIT_D,v)

   def get_iop(self):
      assert self.is_fullword()
      return self.get_bits(*BITS_IOP)

   def set_iop(self,v):
      assert self.is_fullword()
      self.set_bits(*BITS_IOP,v)

   def get_kzotpqr(self):
      assert self.is_fullword()
      return self.get_bits(*BITS_KZOTPQR)

   def set_kzotpqr(self,v):
      assert self.is_fullword()
      self.set_bits(*BITS_KZOTPQR,v)

   def get_abc(self):
      assert self.is_fullword()
      return self.get_bits(*BITS_ABC)

   def set_abc(self,v):
      assert self.is_fullword()
      self.set_bits(*BITS_ABC,v)

   def disasm(self):
      n = [1,2][self.is_halfword()]
      parts = []
      not_an_op = False
      for half in range(n):
         num = self.get_opnum(half)
         nam = OPNUM2NAM[num]
         if nam is None:
            not_an_op = True
            break

         addr = self.get_addr(half)
         indirect = self.get_indirect(half)
         rsp = self.get_rsp(half)
         S = self.get_S(half)

         asm = nam.lower()
         if S: asm += "n"

         if self.is_fullword():
            if self.get_V(): asm += "V"
            if self.get_X(): asm += "X"

         asm += " "

         if indirect: asm += " ("
         if rsp == RSP_ABSOLUTE:
            if addr!=0 or indirect:
               asm += "%d" % addr
            else:
               asm += " "
         elif rsp == RSP_RELATIVE:
            asm += "r%s" % signed10(addr)
         elif rsp == RSP_INDEXED:
            asm += "p%s" % signed10(addr)
         elif rsp == RSP_SEQUENCED:
            asm += "s%s" % signed10(addr)
         else: assert False, "unreachable"

         if indirect: asm += ") "

         if self.is_fullword():
            incr = self.get_incr()
            if incr != 0: asm += " t%s" % signed10(incr)
            if self.get_D(): asm += " D"

            iop = self.get_iop()
            k = self.get_kzotpqr()
            abc = self.get_abc()
            if iop>0 or k>0 or abc>0:
               asm += " "
               asm += IOPS[0][iop]
               if k>0: asm += IOPS[1][k-1]
               if abc>0: asm += IOPS[2][abc-1]

         if self.get_float(): asm += " F"

         parts.append(asm)

      if not_an_op:
         return "; TODO not-an-op %d" % self.word
      else:
         return " , ".join(parts)


def disasm_flx_binary(path):
   data = read_flx(path)

   stripped_header = False
   for header in BIN_HEADERS:
      if data.startswith(header):
         data = data[len(header):]
         stripped_header = True
         break

   assert stripped_header, "invalid format?"

   xs = [x for x in data]

   decode_int = lambda xs: sum([x*((2**7)**i) for i,x in enumerate(xs)])
   get_byte = lambda xs: (xs[0], xs[1:])
   get_int3 = lambda xs: (decode_int(xs[0:3]), xs[3:])
   get_int6 = lambda xs: (decode_int(xs[0:6]), xs[6:])

   def get_bits(v,a,b=None):
      if b is None: b=a
      a=41-a
      b=41-b
      a,b=b,a
      assert a<=b, ("%d<=%d" % (a,b))
      n = (b-a+1)
      return (v>>a) & ((1<<n)-1)

   destination = None

   last_op = None
   while len(xs) > 0:
      x, xs = get_byte(xs)

      if x < 64: # binary block
         for i in range(x):
            x, xs = get_int6(xs)
            op = Op(x)
            print(op.disasm())
            last_op = op
      elif x == 64: # repeat
         x, xs = get_int3(xs)
         assert last_op is not None
         for i in range(x):
            print(last_op.disasm() + " ; (repeat %d)" % i)
      elif x == 65: # destination
         destination, xs = get_int3(xs)
         print("i=%d ; dest=%d" % (destination//1024*40 + destination%1024, destination))
      elif x == 66: #end
         chk, xs = get_int3(xs)
         print("; end chksum", chk//1024, chk%1024)
         break
      else:
         raise RuntimeError("TODO invalid?")

   print("; tail: %s" % repr(gier2str(xs)))

def is_int(str):
   try:
      int(str)
      return True
   except ValueError as e:
      return False

def asm_op(str):
   op = Op()
   halfs = str.split(",")
   num_halfs = len(halfs)
   assert num_halfs>0
   if num_halfs == 1:
      op.make_fullword()
   elif num_halfs == 2:
      op.make_halfword()
   else:
      raise mkerr("too many halfs")

   for half, halfstr in enumerate(halfs):
      items = [x for x in halfstr.split(" ") if len(x)>0]
      assert len(items)>0

      opstr, items = items[0],items[1:]
      if len(opstr)<2: raise mkerr("invalid opstr [%s]" % opstr)
      opnam = opstr[0:2].upper()
      if opnam not in OPNAM2NUM: raise mkerr("unknown opstr [%s]" % opstr)
      opnum = OPNAM2NUM[opnam]
      op.set_opnum(half, opnum)
      tail = opstr[2:]
      while len(tail)>0:
         cc,tail = tail[0],tail[1:]
         if cc=="n" or cc=="S":
            op.set_S(half, 1)
         elif cc=="X":
            op.set_X(1)
         elif cc=="V":
            op.set_V(1)
         else:
            raise mkerr("invalid op suffix")

      while len(items) > 0:
         item, items = items[0], items[1:]
         cc = item[0]
         if cc == "D":
            op.set_D(1)
            continue
         elif 'A'<=cc and cc<='Z':
            st = 0
            while len(item) > 0:
               c,item = item[0],item[1:]
               if st<=0 and c in IOPS[0]:
                  v = IOPS[0].find(c)
                  assert v>=0
                  op.set_iop(v)
                  st=1
               elif st<=1 and c in IOPS[1]:
                  v = IOPS[1].find(c)
                  assert v>=0
                  op.set_kzotpqr(v+1)
                  st=2
               elif st<=2 and c in IOPS[2]:
                  v = IOPS[2].find(c)
                  assert v>=0
                  op.set_abc(v+1)
                  st=3
               else:
                  raise mkerr("invalid indicator char [%s]" % c)
            continue
         elif '('==cc:
            if item[-1] != ")":
               raise mkerr("invalid (...) syntax")
            op.set_indirect(half, 1)
            item = item[1:-1]

         cc = item[0]
         if ('0'<=cc and cc<='9') or cc=='+' or cc=='-':
            op.set_addr(half, int(item))
         elif cc=='r':
            op.set_rsp(half, RSP_RELATIVE)
            if len(item)>1:
               op.set_addr(half, int(item[1:]))
         elif cc=='p':
            op.set_rsp(half, RSP_INDEXED)
            if len(item)>1:
               op.set_addr(half, int(item[1:]))
         elif cc=='s':
            op.set_rsp(half, RSP_SEQUENCED)
            if len(item)>1:
               op.set_addr(half, int(item[1:]))
         elif cc=='t':
            op.set_incr(int(item[1:]))
         else:
            op.set_unresolved_addr_name(half, item)
   return op


def asm_asc(asc_in, flx_out):
   lines = open(asc_in).readlines()
   label2pc = {}

   ops = []

   entry_address = None

   number_mode = None

   pc = 0
   for lineno, line in enumerate(lines):
      line = line.split(";")[0].strip()

      mkerr = lambda msg: RuntimeError("compile error %s at line %d" % (msg, lineno+1))

      if lineno == 0 and line == "slip<":
         continue

      if line.startswith("i="):
         pc = int(line.split("=")[1])
         continue

      if line.startswith("_m"):
         number_mode = "m"
         continue

      if line.startswith("_f"):
         number_mode = "f"
         continue

      if line.startswith("_e"):
         tail = line[2:]
         if tail in label2pc:
            entry_address = label2pc[tail]
         elif len(tail)>0:
            entry_address = int(tail)
         break

      if line.startswith("_b"):
         for x in line[2:].split(","):
            x = x.strip()
            if x.startswith("i="):
               pc = int(x.split("=")[1])
            elif x.startswith("a") or x.startswith("b"):
               pass # ignore label declarations since we allow arbitrary names
            else:
               raise mkerr("bad _b line")
         continue

      if line.startswith("_d"):
         continue # XXX?

      label = None
      xs = line.split(":")
      if len(xs) == 2:
         label, line = xs[0], xs[1].strip()
         label2pc[label] = pc

      if len(line) == 0:
         continue

      op = None
      if ("A"<=line[0].upper()) and (line[0].upper()<="Z"):
         op = asm_op(line)
      elif number_mode=="m":
         mark = 0
         if line[-1] in "abc":
            mark = 1+("abc".find(line[-1]))
            line = line[0:-1]
         if is_int(line):
            op = Op((int(line) << 2) + mark)

      if op is None:
         raise mkerr("invalid line type [%s]" % line)

      assert op is not None
      op.set_pc(pc)
      ops.append(op)
      pc += 1

   for op in ops:
      u = op.get_unresolved_addr_names()
      if len(u)==0: continue
      for name in u:
         if name not in label2pc:
            raise RuntimeError("reference to undefined label [%s]" % name)
         #op.resolve_relative(name, label2pc[name])
         op.resolve_absolute(name, label2pc[name])


   pc0 = ops[0].pc

   def encode_int(num, value):
      bs = b""
      for i in range(num):
         f = (value >> (7*i)) & ((1<<7)-1)
         bs += bytes([f])
      return bs

   def int3(value): return encode_int(3,value)
   def int6(value): return encode_int(6,value)

   segs = [[]]

   last_word = None
   for i,op in enumerate(ops):
      assert (op.pc == pc0+i), "TODO handle segments, padding or something?"
      assert 0==len(op.get_unresolved_addr_names())
      print("[m+%d]"%op.pc, op.disasm())
      word = int6(op.word)

      if word == last_word:
         if type(segs[-1]) is int:
            segs[-1] += 1
         else:
            segs.append(1)
      else:
         if type(segs[-1]) is int: segs.append([])
         ntop = len(segs[-1])
         assert ntop<=63
         if ntop==63: segs.append([])
         segs[-1].append(word)

      last_word = word

   binary = b""
   binary += BIN_HEADERS[0]

   h0 = len(binary)

   binary += bytes([65])
   dest = pc0
   binary += int3(1024*(pc0//40) + pc0%40)
   # the docs say that the bin addr is at:
   #   i _: 1024 × 40 + i _m_o_d 1024  (meaning: i//1024*40 + i%1024)
   # (see https://datamuseum.dk/w/images/1/15/Help3man.pdf page 29)
   # but it doesn't explicitly state you can't just write 41 instead of 1025

   for seg in segs:
      if type(seg) is int:
         binary += bytes([64])
         binary += int3(seg)
      else:
         n = len(seg)
         assert n<64
         binary += bytes([n])
         binary += b"".join(seg)

   binary += bytes([66])
   num_chars = len(binary) - h0
   sum_chars = sum([x for x in binary[h0:]]) % 1024
   binary += int3((num_chars<<10) + (sum_chars))

   if entry_address is not None:
      #binary += str2gier("\nexit,0.0.0.%d,<" % entry_address)
      #binary += (str2gier("\nexit,%d<" % entry_address))
      binary += (bytes([SET_LOWER]) + str2gier("\nexit,%d<" % entry_address))

   write_flx(flx_out, binary)


import sys
prg, argv = sys.argv[0], sys.argv[1:]

def usage():
   out = sys.stderr
   out.write("Usage: %s <cmd> ...\n" % prg)
   out.write("  %s asm <slip.asc> <bin.flx>\n" % prg)
   out.write("  %s disasm <bin.flx>\n" % prg)
   sys.exit(1)

if len(argv) == 0: usage()

cmd, argv = argv[0], argv[1:]

if cmd == "asm":
   asm_asc(*argv)
elif cmd == "disasm":
   disasm_flx_binary(*argv)
else:
   usage()

# TODO
#  - assembly/disassembly of "machine numbers"
#  - definition labels, eg. in ../tapes/demo/nimbistop.asc there's:
#      _d d=270, d1=440, d2=330
#    later referred to like:
#      pa r2 td2-1

PROJECT = flappyzone
PROG_ADDR = 8200
DATA_ADDR = e000

DEVDIR = ../..

LIBDIR = $(DEVDIR)/n80dev
TOOLDIR = $(DEVDIR)/_utils

SDCC = sdcc
CFLAGS = -mz80 -c -I ../ -I $(LIBDIR) -DBUILD_N80
LDFLAGS =  -mz80 --code-loc 0x$(PROG_ADDR) --data-loc 0x$(DATA_ADDR) --no-std-crt0 

LIB_SRC = keyin.c keyfunc.c rand.c beep.c screen.c crtc.c count_vrtc.c bitmap.c font.c bcd_add8.c bcd_to_ascii.c draw_rect.c draw_hlines.c insert_attr3_new.c render_attr_new.c
SRC = $(PROJECT).c
OBJ = mycrt0.rel $(SRC:.c=.rel) $(LIB_SRC:.c=.rel)
IHX_IMAGE = $(PROJECT).ihx
CMT_IMAGE = $(PROJECT).cmt
BIN_IMAGE = $(PROJECT).bin

ZX0_IMAGE = $(PROJECT).zx0
ZX0_CMT_IMAGE = $(PROJECT)_zx0.cmt

all: $(CMT_IMAGE)

mycrt0.rel: $(LIBDIR)/mycrt0.asm
	sdasz80 -o $@ $<

%.rel: ../%.c
	$(SDCC) $(CFLAGS) $<

%.rel: $(LIBDIR)/%.c
	$(SDCC) $(CFLAGS) $<

$(IHX_IMAGE): $(OBJ)
	$(SDCC) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

$(BIN_IMAGE): $(IHX_IMAGE)
	$(TOOLDIR)/hex2bin $<

$(CMT_IMAGE): $(BIN_IMAGE)
	$(TOOLDIR)/t88tool -M -f $@ $< $(PROG_ADDR)

zx0: $(ZX0_CMT_IMAGE)

$(ZX0_IMAGE): $(BIN_IMAGE)
	rm -f *.zx0
	$(TOOLDIR)/zx0 $< $@

$(ZX0_CMT_IMAGE): $(ZX0_IMAGE)
	$(TOOLDIR)/bin2cmt.py $< $(PROG_ADDR) -e E400 -o $@

clean:
	rm -rf $(OBJ) $(IHX_IMAGE) $(CMT_IMAGE) $(ZX0_CMT_IMAGE) *.lst *.sym *.asm *.lk *.noi *.map *.n80 *.zx0 *.bin

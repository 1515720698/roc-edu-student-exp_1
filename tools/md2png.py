import os
import sys
from PIL import Image, ImageDraw, ImageFont

FONT_PATH = '/usr/share/fonts/wqy-zenhei/wqy-zenhei.ttc'
FONT_SIZE = 16
LINE_H = 24
PAD = 12
TITLE_H = 32
WRAP = 100


def wrap(line, n=WRAP):
    if not line:
        return [' ']
    return [line[i:i + n] for i in range(0, len(line), n)]


def render(lines, caption, out):
    font = ImageFont.truetype(FONT_PATH, FONT_SIZE, index=0)
    tfont = ImageFont.truetype(FONT_PATH, 14, index=0)
    rows = []
    for l in lines:
        rows.extend(wrap(l))
    width = max([font.getlength(r) for r in rows] + [tfont.getlength(caption), 400]) + PAD * 2
    height = len(rows) * LINE_H + PAD * 2 + TITLE_H
    img = Image.new('RGB', (int(width), int(height)), (24, 24, 24))
    d = ImageDraw.Draw(img)
    d.rectangle([0, 0, width, TITLE_H], fill=(58, 58, 58))
    d.text((PAD, 8), caption, font=tfont, fill=(235, 235, 235))
    y = TITLE_H + PAD
    for r in rows:
        d.text((PAD, y), r, font=font, fill=(215, 215, 215))
        y += LINE_H
    img.save(out)
    print(out)


def main():
    src, outdir = sys.argv[1], sys.argv[2]
    os.makedirs(outdir, exist_ok=True)
    stem = os.path.splitext(os.path.basename(src))[0]
    buf, in_code, heading, idx = [], False, '', 0
    for ln in open(src, encoding='utf-8').read().split('\n'):
        if ln.startswith('```'):
            if in_code:
                idx += 1
                caption = '%s #%d  %s' % (stem, idx, heading[:60])
                render(buf, caption, os.path.join(outdir, '%s_%d.png' % (stem, idx)))
                buf = []
            in_code = not in_code
            continue
        if in_code:
            buf.append(ln.rstrip('\r'))
        elif ln.startswith('#'):
            heading = ln.lstrip('#').strip()
    print('total', idx)


main()
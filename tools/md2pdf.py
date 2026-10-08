import sys
from reportlab.lib.pagesizes import A4
from reportlab.lib.units import mm
from reportlab.lib.styles import ParagraphStyle
from reportlab.lib.colors import HexColor
import os
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Preformatted

FONT = 'CJK'
_candidates = [
    '/usr/share/fonts/cjkuni-uming/uming.ttc',
    '/usr/share/fonts/wqy-zenhei/wqy-zenhei.ttc',
    '/usr/share/fonts/google-noto-sans-cjk-sc/NotoSansCJKsc-Regular.otf',
]
for i, p in enumerate(_candidates):
    if os.path.exists(p):
        pdfmetrics.registerFont(TTFont(FONT, p, subfontIndex=0))
        break
else:
    raise SystemExit('no CJK font found')

body = ParagraphStyle('body', fontName=FONT, fontSize=10.5, leading=16)
h1 = ParagraphStyle('h1', parent=body, fontSize=18, leading=26, spaceBefore=12, spaceAfter=8)
h2 = ParagraphStyle('h2', parent=body, fontSize=15, leading=22, spaceBefore=10, spaceAfter=6)
h3 = ParagraphStyle('h3', parent=body, fontSize=12.5, leading=19, spaceBefore=8, spaceAfter=4)
code = ParagraphStyle('code', fontName=FONT, fontSize=8.5, leading=12.5, leftIndent=6)
quote = ParagraphStyle('quote', parent=body, leftIndent=12, textColor=HexColor('#555555'))


def esc(s):
    return s.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;')


def clean(s):
    return esc(s.replace('**', '').replace('`', ''))


def wrap(line, width=92):
    return '\n'.join(line[i:i + width] for i in range(0, len(line), width)) or ' '


def build(src, dst):
    story = []
    in_code = False
    buf = []
    with open(src, encoding='utf-8') as f:
        for line in f:
            line = line.rstrip('\n')
            if line.startswith('```'):
                if in_code:
                    story.append(Preformatted('\n'.join(buf), code))
                    buf = []
                in_code = not in_code
                continue
            if in_code:
                buf.append(wrap(line))
                continue
            if not line.strip():
                story.append(Spacer(1, 4))
            elif line.startswith('# '):
                story.append(Paragraph(clean(line[2:]), h1))
            elif line.startswith('## '):
                story.append(Paragraph(clean(line[3:]), h2))
            elif line.startswith('### '):
                story.append(Paragraph(clean(line[4:]), h3))
            elif line.startswith('> '):
                story.append(Paragraph(clean(line[2:]), quote))
            elif line.startswith('- '):
                story.append(Paragraph('• ' + clean(line[2:]), body))
            else:
                story.append(Paragraph(clean(line), body))
    if buf:
        story.append(Preformatted('\n'.join(buf), code))
    SimpleDocTemplate(dst, pagesize=A4, leftMargin=18 * mm, rightMargin=18 * mm,
                      topMargin=16 * mm, bottomMargin=16 * mm, title=src).build(story)


for a in sys.argv[1:]:
    out = a[:-3] + '.pdf'
    build(a, out)
    print('built', out)

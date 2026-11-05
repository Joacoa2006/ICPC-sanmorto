#!/usr/bin/env python3
"""
Genera un PDF A4 apaisado, 2 columnas, pensado para imprimir un notebook ICPC.

Objetivo de este layout "packed-v7-auto":
- Incluye automaticamente TODOS los archivos .cpp del repo.
- Busca tambien en subdirectorios.
- NO fuerza página nueva al cambiar de sección.
- NO fuerza columna nueva al cambiar de archivo.
- El código se parte automáticamente entre columnas/páginas.
- No usa KeepTogether/keepWithNext para bloques de código.
- Espaciado vertical mínimo: los títulos son los separadores.

Uso:
    python generar_notebook_icpc_packed_v7_auto.py . -o ICPC-sanmorto.pdf

Opcional:
    --compact   quita main() de prueba de cada archivo, excepto template.cpp.

Dependencia:
    pip install reportlab pygments
"""

from __future__ import annotations

import argparse
from datetime import datetime
from pathlib import Path

from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER
from reportlab.lib.pagesizes import A4, landscape
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import mm
from reportlab.platypus import (
    BaseDocTemplate,
    Frame,
    PageTemplate,
    Paragraph,
    Preformatted,
    Spacer,
    PageBreak,
)
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus.xpreformatted import XPreformatted

from pygments import lex
from pygments.lexers import CppLexer
from pygments.token import Token
from xml.sax.saxutils import escape


# -----------------------------------------------------------------------------
# ORGANIZACION AUTOMATICA
#
# Se buscan TODOS los .cpp del repo, incluso dentro de subdirectorios.
# La clasificacion es solamente estetica: si un nombre no coincide con ninguna
# regla, el archivo entra en "Otros codigos". Por lo tanto ningun .cpp queda
# afuera del handbook por no estar listado a mano.
# -----------------------------------------------------------------------------
SECTION_ORDER = [
    "1. Base y utilidades",
    "2. Matematica y teoria de numeros",
    "3. Estructuras de datos y range queries",
    "4. Grafos, arboles y MST",
    "5. Caminos minimos",
    "6. Matching y flujo",
    "7. Strings",
    "8. Geometria",
    "9. Otros codigos",
]

PRETTY_TITLES = {
    "template.cpp": "Template base",
    "binary_search.cpp": "Binary Search",
    "int128.cpp": "__int128",
    "combinatorio.cpp": "Combinatoria",
    "sieve.cpp": "Sieve",
    "spf.cpp": "Smallest Prime Factor (SPF)",
    "pollard_rho.cpp": "Pollard Rho",
    "mat_expo.cpp": "Exponentiacion de matrices",
    "fenwick.cpp": "Fenwick Tree",
    "fenwick_2d.cpp": "Fenwick Tree 2D",
    "segment_tree.cpp": "Segment Tree",
    "lazy_segment_tree.cpp": "Lazy Segment Tree",
    "sparse_table.cpp": "Sparse Table",
    "ordered_set.cpp": "Ordered Set / PBDS",
    "mo.cpp": "Mo's Algorithm",
    "bfs_dfs.cpp": "BFS / DFS",
    "uf_kruskal.cpp": "Union Find + Kruskal",
    "binary_lifting.cpp": "Binary Lifting",
    "dijkstra.cpp": "Dijkstra",
    "dijsktra.cpp": "Dijkstra",
    "bellman_ford.cpp": "Bellman-Ford",
    "floyd_warshall.cpp": "Floyd-Warshall",
    "matching.cpp": "Kuhn - Matching bipartito",
    "kuhn.cpp": "Kuhn - Matching bipartito",
    "blossom.cpp": "Blossom - Matching general",
    "dinic.cpp": "Dinic - Maximum Flow",
    "hash.cpp": "String Hash",
    "string_hash.cpp": "String Hash",
    "geometry.cpp": "Geometria 2D",
    "geometry_basic.cpp": "Geometria 2D basica",
}


def pretty_title(path: Path) -> str:
    if path.name in PRETTY_TITLES:
        return PRETTY_TITLES[path.name]
    return path.stem.replace("_", " ").replace("-", " ").title()


def classify_cpp(path: Path) -> str:
    s = path.as_posix().lower()
    name = path.name.lower()

    # Strings
    if any(x in s for x in (
        "hash", "string", "kmp", "manacher", "suffix", "trie",
        "aho", "z_function", "zfunction", "rolling_hash"
    )):
        return "7. Strings"

    # Geometria
    if any(x in s for x in (
        "geometry", "geometr", "convex", "polygon", "circle",
        "circun", "point_in", "line_inter", "segment_inter"
    )):
        return "8. Geometria"

    # Matching / flujo
    if any(x in s for x in (
        "dinic", "flow", "matching", "kuhn", "blossom", "hopcroft",
        "hungarian", "min_cost"
    )):
        return "6. Matching y flujo"

    # Caminos minimos
    if any(x in s for x in (
        "dijkstra", "dijsktra", "bellman", "floyd", "shortest"
    )):
        return "5. Caminos minimos"

    # Grafos / arboles / MST
    if any(x in s for x in (
        "bfs", "dfs", "kruskal", "union_find", "unionfind", "dsu",
        "binary_lifting", "lca", "mst", "toposort", "topological",
        "scc", "bridge", "articulation", "euler", "centroid", "hld"
    )):
        return "4. Grafos, arboles y MST"

    # Estructuras / range queries
    if any(x in s for x in (
        "fenwick", "segment_tree", "segtree", "sparse_table", "ordered_set",
        "pbds", "mo.cpp", "/mo/", "merge_sort_tree", "persistent", "treap",
        "wavelet", "lazy"
    )):
        return "3. Estructuras de datos y range queries"

    # Matematica / teoria de numeros
    if any(x in s for x in (
        "combin", "sieve", "spf", "pollard", "prime", "factor", "gcd",
        "modular", "crt", "matrix_expo", "mat_expo", "fft", "ntt",
        "number_theory"
    )):
        return "2. Matematica y teoria de numeros"

    # Base / utilidades
    if name in {"template.cpp", "binary_search.cpp", "int128.cpp"} or any(
        x in s for x in ("rotate", "rotation", "flip", "grid_transform", "utility", "utils")
    ):
        return "1. Base y utilidades"

    return "9. Otros codigos"


def discover_sections(repo: Path):
    ignored_dirs = {".git", ".github", "build", "dist", "__pycache__"}
    files = []

    for p in repo.rglob("*.cpp"):
        rel = p.relative_to(repo)
        if any(part in ignored_dirs for part in rel.parts):
            continue
        files.append(p)

    files.sort(key=lambda p: p.relative_to(repo).as_posix().lower())

    grouped = {section: [] for section in SECTION_ORDER}
    for p in files:
        grouped[classify_cpp(p.relative_to(repo))].append((pretty_title(p), p))

    return [(section, grouped[section]) for section in SECTION_ORDER if grouped[section]]

# -----------------------------------------------------------------------------
# LAYOUT
# -----------------------------------------------------------------------------
PAGE = landscape(A4)
PAGE_W, PAGE_H = PAGE

MARGIN_X = 8 * mm
MARGIN_TOP = 9 * mm
MARGIN_BOTTOM = 10 * mm
GUTTER = 4.5 * mm

# Código: DejaVu Sans Mono es más legible en impresión que Courier.
# Se registra desde una ruta estándar de Linux (GitHub Actions usa Ubuntu).
DEJAVU_MONO = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
DEJAVU_MONO_BOLD = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf"
CODE_FONT = "DejaVuSansMono"
CODE_FONT_BOLD = "DejaVuSansMono-Bold"
CODE_SIZE = 7.1
CODE_LEADING = 8.05
MAX_LINE_LENGTH = 92
LAYOUT_VERSION = "packed-v7-auto"

# Escala de grises pensada para impresión.
GRAY_COMMENT = "#8A8A8A"
GRAY_LITERAL = "#4F4F4F"
GRAY_PREPROC = "#303030"


def register_code_fonts():
    regular = Path(DEJAVU_MONO)
    bold = Path(DEJAVU_MONO_BOLD)

    if regular.exists():
        pdfmetrics.registerFont(TTFont(CODE_FONT, str(regular)))
        regular_name = CODE_FONT
    else:
        print("Aviso: DejaVu Sans Mono no encontrado; se usa Courier.")
        regular_name = "Courier"

    if bold.exists():
        pdfmetrics.registerFont(TTFont(CODE_FONT_BOLD, str(bold)))
        bold_name = CODE_FONT_BOLD
    else:
        bold_name = "Courier-Bold"

    return regular_name, bold_name


def wrap_code_lines(code: str, width: int = MAX_LINE_LENGTH) -> str:
    """Replica el wrapping simple de Preformatted antes de aplicar colores."""
    # Caracteres naturales en los que conviene partir una línea de C++.
    split_chars = " \t,.;:+-*/%&|^=!<>?()[]{}"
    out = []

    for line in code.split("\n"):
        rest = line
        first = True

        while len(rest) > width:
            cut = 0
            for i in range(min(width, len(rest))):
                if rest[i] in split_chars:
                    cut = i + 1
            if cut == 0:
                cut = width

            out.append(rest[:cut])
            rest = rest[cut:]
            first = False

        out.append(rest)

    return "\n".join(out)


def cpp_to_reportlab_markup(code: str, regular_font: str, bold_font: str) -> str:
    """Syntax highlighting C++ en escala de grises para XPreformatted."""
    code = wrap_code_lines(code)
    parts = []

    for token_type, text in lex(code, CppLexer()):
        safe = escape(text)
        if not safe:
            continue

        # Preprocesador: oscuro y en negrita. Debe evaluarse antes que Comment,
        # porque Pygments clasifica #define/#include como Comment.Preproc.
        if token_type in Token.Comment.Preproc:
            parts.append(
                f'<font name="{bold_font}" color="{GRAY_PREPROC}">{safe}</font>'
            )
        # Comentarios normales: gris claro, como en notebooks ICPC impresos.
        elif token_type in Token.Comment:
            parts.append(
                f'<font name="{regular_font}" color="{GRAY_COMMENT}">{safe}</font>'
            )
        # Keywords y tipos: negro + bold para encontrarlos de un vistazo.
        elif token_type in Token.Keyword:
            parts.append(
                f'<font name="{bold_font}" color="#000000">{safe}</font>'
            )
        # Literales: gris oscuro, distinto sin perder contraste al imprimir.
        elif token_type in Token.Literal:
            parts.append(
                f'<font name="{regular_font}" color="{GRAY_LITERAL}">{safe}</font>'
            )
        else:
            parts.append(safe)

    return "".join(parts)


class NotebookDoc(BaseDocTemplate):
    pass


def strip_test_main(code: str, filename: str) -> str:
    """Quita el main de prueba si aparece al final del archivo."""
    if filename == "template.cpp":
        return code

    lines = code.splitlines()
    for i, line in enumerate(lines):
        s = line.lstrip()
        if s.startswith("int main(") or s.startswith("int main ("):
            lines = lines[:i]
            break

    while lines and not lines[-1].strip():
        lines.pop()

    return "\n".join(lines)


def read_code(path: Path, compact: bool) -> str:
    code = path.read_text(encoding="utf-8", errors="replace")
    code = code.replace("\r\n", "\n").replace("\r", "\n").replace("\t", "    ")
    if compact:
        code = strip_test_main(code, path.name)
    return code


def on_page(canvas, doc):
    canvas.saveState()

    canvas.setStrokeColor(colors.HexColor("#B8B8B8"))
    canvas.setLineWidth(0.35)
    canvas.line(MARGIN_X, PAGE_H - 6.7 * mm, PAGE_W - MARGIN_X, PAGE_H - 6.7 * mm)

    canvas.setFont("Helvetica", 6.6)
    canvas.setFillColor(colors.HexColor("#555555"))
    canvas.drawString(
        MARGIN_X,
        5.2 * mm,
        f"ICPC Sanmorto - Notebook | layout {LAYOUT_VERSION}",
    )
    canvas.drawRightString(PAGE_W - MARGIN_X, 5.2 * mm, f"Página {doc.page}")

    canvas.restoreState()


def build_pdf(repo: Path, output: Path, compact: bool):
    code_font, code_font_bold = register_code_fonts()
    styles = getSampleStyleSheet()

    title_style = ParagraphStyle(
        "NotebookTitle",
        parent=styles["Title"],
        fontName="Helvetica-Bold",
        fontSize=19,
        leading=21,
        alignment=TA_CENTER,
        spaceAfter=3,
    )

    subtitle_style = ParagraphStyle(
        "Subtitle",
        parent=styles["Normal"],
        fontName="Helvetica",
        fontSize=7.5,
        leading=9,
        alignment=TA_CENTER,
        textColor=colors.HexColor("#444444"),
        spaceAfter=4,
    )

    # IMPORTANTE: keepWithNext=False.
    # Una sección puede arrancar al final de cualquier columna.
    section_style = ParagraphStyle(
        "Section",
        parent=styles["Heading1"],
        fontName="Helvetica-Bold",
        fontSize=12.0,
        leading=13.5,
        textColor=colors.black,
        borderWidth=0,
        spaceBefore=2,
        spaceAfter=1.5,
        keepWithNext=False,
    )

    # IMPORTANTE: tampoco se mantiene el título pegado al bloque completo.
    file_style = ParagraphStyle(
        "File",
        parent=styles["Heading2"],
        fontName="Helvetica-Bold",
        fontSize=8.7,
        leading=9.8,
        spaceBefore=1,
        spaceAfter=0.5,
        keepWithNext=False,
    )

    index_section_style = ParagraphStyle(
        "IndexSection",
        parent=styles["Normal"],
        fontName="Helvetica-Bold",
        fontSize=8.0,
        leading=9.2,
        spaceBefore=1.8,
        spaceAfter=0.2,
    )

    index_item_style = ParagraphStyle(
        "IndexItem",
        parent=styles["Normal"],
        fontName="Helvetica",
        fontSize=7.0,
        leading=8.0,
        leftIndent=3.5 * mm,
        spaceBefore=0,
        spaceAfter=0,
    )

    code_style = ParagraphStyle(
        "Code",
        fontName=code_font,
        fontSize=CODE_SIZE,
        leading=CODE_LEADING,
        leftIndent=0,
        rightIndent=0,
        spaceBefore=0,
        spaceAfter=0,
        textColor=colors.black,
        allowWidows=1,
        allowOrphans=1,
    )

    usable_w = PAGE_W - 2 * MARGIN_X
    col_w = (usable_w - GUTTER) / 2
    frame_y = MARGIN_BOTTOM
    frame_h = PAGE_H - MARGIN_TOP - MARGIN_BOTTOM

    frames = [
        Frame(
            MARGIN_X,
            frame_y,
            col_w,
            frame_h,
            id="col1",
            leftPadding=0,
            rightPadding=0,
            topPadding=0,
            bottomPadding=0,
        ),
        Frame(
            MARGIN_X + col_w + GUTTER,
            frame_y,
            col_w,
            frame_h,
            id="col2",
            leftPadding=0,
            rightPadding=0,
            topPadding=0,
            bottomPadding=0,
        ),
    ]

    doc = NotebookDoc(
        str(output),
        pagesize=PAGE,
        leftMargin=MARGIN_X,
        rightMargin=MARGIN_X,
        topMargin=MARGIN_TOP,
        bottomMargin=MARGIN_BOTTOM,
        title="ICPC Sanmorto - Notebook",
        author="ICPC Sanmorto",
    )
    doc.addPageTemplates(PageTemplate(id="two_columns", frames=frames, onPage=on_page))

    # Descubrir automaticamente todos los .cpp del repo.
    resolved_sections = discover_sections(repo)
    cpp_count = sum(len(entries) for _, entries in resolved_sections)

    if cpp_count == 0:
        raise SystemExit(f"No se encontraron archivos .cpp en {repo}")

    story = []

    # -------------------------------------------------------------------------
    # PORTADA / ÍNDICE
    # El índice queda separado del código: al terminar, comienza una hoja nueva.
    # -------------------------------------------------------------------------
    story.append(Spacer(1, 3 * mm))
    story.append(Paragraph("ICPC Sanmorto", title_style))
    story.append(Paragraph("Notebook de programación competitiva", title_style))

    mode = "compacto (sin mains de prueba)" if compact else "completo"
    story.append(
        Paragraph(
            f"Generado desde <b>{repo.name}</b> - modo {mode} - "
            f"{datetime.now().strftime('%Y-%m-%d')}",
            subtitle_style,
        )
    )

    story.append(Paragraph("Índice", section_style))

    for section_name, entries in resolved_sections:
        story.append(Paragraph(section_name, index_section_style))
        for idx, (title, p) in enumerate(entries, 1):
            story.append(
                Paragraph(
                    f"{idx}. {title} <font color='#666666'>({p.relative_to(repo).as_posix()})</font>",
                    index_item_style,
                )
            )

    # El código siempre empieza en una hoja nueva después del índice.
    story.append(PageBreak())

    # -------------------------------------------------------------------------
    # CÓDIGOS - FLUJO CONTINUO ENTRE SECCIONES
    # Después del índice NO hay más PageBreak/FrameBreak entre secciones.
    # Preformatted se puede partir por líneas y aprovecha el resto de columna.
    # Se deja un pequeño espacio después de cada archivo para separarlos visualmente.
    # -------------------------------------------------------------------------
    for section_name, entries in resolved_sections:
        story.append(Paragraph(section_name, section_style))

        for title, path in entries:
            story.append(
                Paragraph(
                    f"{title} <font color='#666666'>- {path.relative_to(repo).as_posix()}</font>",
                    file_style,
                )
            )

            code = read_code(path, compact)
            if not code.strip():
                code = "// Archivo vacío en modo compacto"

            highlighted = cpp_to_reportlab_markup(code, code_font, code_font_bold)
            story.append(
                XPreformatted(
                    highlighted,
                    code_style,
                )
            )

            # Aproximadamente 1-2 líneas de código de separación entre archivos.
            story.append(Spacer(1, 3.2 * mm))

    output.parent.mkdir(parents=True, exist_ok=True)
    doc.build(story)

    print(f"PDF generado: {output}")
    print(f"Layout: {LAYOUT_VERSION}")

    print(f"Codigos .cpp incluidos: {cpp_count}")


def main():
    parser = argparse.ArgumentParser(
        description="Genera un notebook PDF A4 horizontal de dos columnas"
    )
    parser.add_argument(
        "repo",
        nargs="?",
        default=".",
        help="carpeta raíz del repo (default: .)",
    )
    parser.add_argument(
        "-o",
        "--output",
        default="ICPC-sanmorto-notebook.pdf",
        help="PDF de salida",
    )
    parser.add_argument(
        "--compact",
        action="store_true",
        help="quita main() de prueba de cada archivo excepto template.cpp",
    )

    args = parser.parse_args()

    repo = Path(args.repo).resolve()
    output = Path(args.output).resolve()

    if not repo.is_dir():
        raise SystemExit(f"No existe la carpeta: {repo}")

    build_pdf(repo, output, args.compact)


if __name__ == "__main__":
    main()

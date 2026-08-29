#!/usr/bin/env python3
"""
Genera un PDF imprimible a partir del repo ICPC-sanmorto.

Uso:
    python generar_notebook_icpc.py . -o ICPC-sanmorto.pdf --compact

Dependencia:
    pip install reportlab

--compact elimina el main() de prueba de cada archivo (excepto template.cpp),
asumiendo que main() está al final del archivo.
"""

from __future__ import annotations

import argparse
from pathlib import Path
from datetime import datetime

from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER
from reportlab.lib.pagesizes import A4, landscape
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import mm
from reportlab.platypus import (
    BaseDocTemplate,
    Frame,
    PageBreak,
    PageTemplate,
    Paragraph,
    Preformatted,
    Spacer,
)


# -----------------------------------------------------------------------------
# ORDEN DEL NOTEBOOK
# Para agregar/reordenar códigos, editar solamente esta lista.
# Cada entrada admite varios nombres de archivo alternativos.
# -----------------------------------------------------------------------------
SECTIONS = [
    (
        "1. Base y utilidades",
        [
            ("Template base", ("template.cpp",)),
            ("Binary search", ("binary_search.cpp",)),
            ("__int128", ("int128.cpp",)),
        ],
    ),
    (
        "2. Matemática y teoría de números",
        [
            ("Combinatoria", ("combinatorio.cpp",)),
            ("Sieve", ("sieve.cpp",)),
            ("Smallest Prime Factor (SPF)", ("spf.cpp",)),
            ("Pollard Rho", ("pollard_rho.cpp",)),
            ("Exponentiación de matrices", ("mat_expo.cpp",)),
        ],
    ),
    (
        "3. Estructuras de datos y range queries",
        [
            ("Fenwick Tree", ("fenwick.cpp",)),
            ("Fenwick Tree 2D", ("fenwick_2d.cpp",)),
            ("Segment Tree", ("segment_tree.cpp",)),
            ("Sparse Table", ("sparse_table.cpp",)),
            ("Ordered Set / PBDS", ("ordered_set.cpp",)),
            ("Mo's Algorithm", ("mo.cpp",)),
        ],
    ),
    (
        "4. Grafos básicos, árboles y MST",
        [
            ("BFS / DFS", ("bfs_dfs.cpp",)),
            ("Union Find + Kruskal", ("uf_kruskal.cpp",)),
            ("Binary Lifting", ("binary_lifting.cpp",)),
        ],
    ),
    (
        "5. Caminos mínimos",
        [
            ("Dijkstra", ("dijkstra.cpp", "dijsktra.cpp")),
            ("Bellman-Ford", ("bellman_ford.cpp",)),
            ("Floyd-Warshall", ("floyd_warshall.cpp",)),
        ],
    ),
    (
        "6. Matching y flujo",
        [
            ("Kuhn - Matching bipartito", ("matching.cpp", "kuhn.cpp")),
            ("Blossom - Matching general", ("blossom.cpp",)),
            ("Dinic - Maximum Flow", ("dinic.cpp",)),
        ],
    ),
]


PAGE = landscape(A4)
PAGE_W, PAGE_H = PAGE
MARGIN_X = 9 * mm
MARGIN_TOP = 10 * mm
MARGIN_BOTTOM = 11 * mm
GUTTER = 5 * mm

CODE_FONT = "Courier"
CODE_SIZE = 6.25
CODE_LEADING = 7.15
MAX_LINE_LENGTH = 104


class NotebookDoc(BaseDocTemplate):
    pass


def resolve_file(repo: Path, alternatives: tuple[str, ...]) -> Path | None:
    for name in alternatives:
        p = repo / name
        if p.exists():
            return p
    return None


def strip_test_main(code: str, filename: str) -> str:
    """Compacta el notebook quitando main() de prueba si está al final."""
    if filename == "template.cpp":
        return code

    lines = code.splitlines()
    for i, line in enumerate(lines):
        if line.lstrip().startswith("int main(") or line.lstrip().startswith("int main ("):
            lines = lines[:i]
            break

    while lines and not lines[-1].strip():
        lines.pop()
    return "\n".join(lines)


def read_code(path: Path, compact: bool) -> str:
    code = path.read_text(encoding="utf-8", errors="replace")
    code = code.replace("\t", "    ").replace("\r\n", "\n").replace("\r", "\n")
    if compact:
        code = strip_test_main(code, path.name)
    return code


def on_page(canvas, doc):
    canvas.saveState()

    # Línea superior y pie discretos, pensados para impresión B/N.
    canvas.setStrokeColor(colors.HexColor("#B8B8B8"))
    canvas.setLineWidth(0.35)
    canvas.line(MARGIN_X, PAGE_H - 7.3 * mm, PAGE_W - MARGIN_X, PAGE_H - 7.3 * mm)

    canvas.setFont("Helvetica", 6.8)
    canvas.setFillColor(colors.HexColor("#555555"))
    canvas.drawString(MARGIN_X, 5.7 * mm, "ICPC Sanmorto - Notebook")
    canvas.drawRightString(PAGE_W - MARGIN_X, 5.7 * mm, f"Página {doc.page}")

    canvas.restoreState()


def build_pdf(repo: Path, output: Path, compact: bool):
    styles = getSampleStyleSheet()

    title_style = ParagraphStyle(
        "NotebookTitle",
        parent=styles["Title"],
        fontName="Helvetica-Bold",
        fontSize=21,
        leading=23,
        alignment=TA_CENTER,
        spaceAfter=6,
    )
    subtitle_style = ParagraphStyle(
        "Subtitle",
        parent=styles["Normal"],
        fontName="Helvetica",
        fontSize=8,
        leading=10,
        alignment=TA_CENTER,
        textColor=colors.HexColor("#444444"),
        spaceAfter=8,
    )
    section_style = ParagraphStyle(
        "Section",
        parent=styles["Heading1"],
        fontName="Helvetica-Bold",
        fontSize=13,
        leading=15,
        spaceBefore=2,
        spaceAfter=7,
        keepWithNext=True,
    )
    file_style = ParagraphStyle(
        "File",
        parent=styles["Heading2"],
        fontName="Helvetica-Bold",
        fontSize=8.6,
        leading=10.2,
        spaceBefore=4,
        spaceAfter=2,
        keepWithNext=True,
    )
    index_section_style = ParagraphStyle(
        "IndexSection",
        parent=styles["Normal"],
        fontName="Helvetica-Bold",
        fontSize=8.3,
        leading=10,
        spaceBefore=3,
        spaceAfter=1,
    )
    index_item_style = ParagraphStyle(
        "IndexItem",
        parent=styles["Normal"],
        fontName="Helvetica",
        fontSize=7.2,
        leading=8.7,
        leftIndent=4 * mm,
        spaceAfter=0,
    )
    code_style = ParagraphStyle(
        "Code",
        fontName=CODE_FONT,
        fontSize=CODE_SIZE,
        leading=CODE_LEADING,
        leftIndent=0,
        rightIndent=0,
        spaceBefore=0,
        spaceAfter=4,
        textColor=colors.black,
        allowWidows=1,
        allowOrphans=1,
    )

    usable_w = PAGE_W - 2 * MARGIN_X
    col_w = (usable_w - GUTTER) / 2
    frame_y = MARGIN_BOTTOM
    frame_h = PAGE_H - MARGIN_TOP - MARGIN_BOTTOM

    frames = [
        Frame(MARGIN_X, frame_y, col_w, frame_h, id="col1", leftPadding=0, rightPadding=0, topPadding=0, bottomPadding=0),
        Frame(MARGIN_X + col_w + GUTTER, frame_y, col_w, frame_h, id="col2", leftPadding=0, rightPadding=0, topPadding=0, bottomPadding=0),
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

    # Resolver archivos antes de construir el índice.
    resolved_sections = []
    used_files: set[str] = set()
    missing: list[str] = []

    for section_name, entries in SECTIONS:
        resolved_entries = []
        for title, alternatives in entries:
            p = resolve_file(repo, alternatives)
            if p is None:
                missing.append(" / ".join(alternatives))
                continue
            resolved_entries.append((title, p))
            used_files.add(p.name)
        if resolved_entries:
            resolved_sections.append((section_name, resolved_entries))

    # Todo .cpp nuevo que todavía no esté en el manifiesto se imprime al final.
    extras = [p for p in sorted(repo.glob("*.cpp")) if p.name not in used_files]
    if extras:
        resolved_sections.append(("7. Otros códigos", [(p.stem, p) for p in extras]))

    story = []

    # Portada / índice
    story.append(Spacer(1, 6 * mm))
    story.append(Paragraph("ICPC Sanmorto", title_style))
    story.append(Paragraph("Notebook de programación competitiva", title_style))
    mode = "compacto (sin mains de prueba)" if compact else "completo"
    story.append(Paragraph(
        f"Generado desde <b>{repo.name}</b> - modo {mode} - {datetime.now().strftime('%Y-%m-%d')}",
        subtitle_style,
    ))
    story.append(Spacer(1, 3 * mm))
    story.append(Paragraph("Índice", section_style))

    for section_name, entries in resolved_sections:
        story.append(Paragraph(section_name, index_section_style))
        for idx, (title, p) in enumerate(entries, 1):
            story.append(Paragraph(f"{idx}. {title} <font color='#666666'>({p.name})</font>", index_item_style))

    if missing:
        story.append(Spacer(1, 3 * mm))
        story.append(Paragraph(
            "Archivos del manifiesto no encontrados: " + ", ".join(missing),
            ParagraphStyle(
                "Warning", parent=index_item_style, textColor=colors.HexColor("#777777"), leftIndent=0
            ),
        ))

    # Códigos
    for sec_idx, (section_name, entries) in enumerate(resolved_sections):
        story.append(PageBreak())
        story.append(Paragraph(section_name, section_style))

        for file_idx, (title, path) in enumerate(entries, 1):
            story.append(Paragraph(f"{title}  <font color='#666666'>- {path.name}</font>", file_style))
            code = read_code(path, compact)
            if not code.strip():
                code = "// Archivo vacío en modo compacto"
            story.append(Preformatted(code, code_style, maxLineLength=MAX_LINE_LENGTH))
            story.append(Spacer(1, 1.5 * mm))

    output.parent.mkdir(parents=True, exist_ok=True)
    doc.build(story)

    print(f"PDF generado: {output}")
    if missing:
        print("Aviso: faltaron archivos del manifiesto:")
        for x in missing:
            print("  -", x)
    if extras:
        print("Archivos no listados agregados en 'Otros códigos':")
        for p in extras:
            print("  -", p.name)


def main():
    parser = argparse.ArgumentParser(description="Genera un notebook PDF imprimible desde ICPC-sanmorto")
    parser.add_argument("repo", nargs="?", default=".", help="carpeta raíz del repo (default: .)")
    parser.add_argument("-o", "--output", default="ICPC-sanmorto-notebook.pdf", help="PDF de salida")
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

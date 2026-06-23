# AGENTS.md — latex_auto 项目指南

## 项目目的
将 CUMCM 国赛数学建模论文从 Word 文档自动排版为 LaTeX PDF。

## 工作方式
- 用户提供：paper.docx（Word 论文正文，已用标题样式标注章节）+ figures/ + code/ + ref.bib
- Agent 负责：将内容提取、转换为 LaTeX，编译输出排版好的 PDF
- 每次一个项目文件夹，放在本目录下

## 关键规范
- 模板类：cumcmthesis.cls（从 `D:\BaiduNetdiskDownload\practice\bilibili\template\` 复制）
- 编译器：XeLaTeX（路径 `D:\APP\texlive\2026\bin\windows\xelatex.exe`）
- 参考示例：`D:\BaiduNetdiskDownload\practice\bilibili\template\example.tex`
- 论文语言：纯中文
- 代码用 lstlisting 环境，支持 Python/MATLAB

## 开工前必读
1. `.codex/KNOWLEDGE.md` — 环境信息、已知问题、约定
2. `scripts/` — 工具脚本

## 约定
- Word "标题1" → \section，"标题2" → \subsection，"标题3" → \subsubsection
- 公式用 Word 公式编辑器（Alt+=），不用截图
- 图片放 figures/，代码放 code/，参考文献 ref.bib

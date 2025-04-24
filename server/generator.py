def convert_html_to_cpp(html_file_path, output_file_path):

    with open(html_file_path, "r", encoding="utf-8") as file:
        html_content = file.read()

    html_content = html_content.replace("\\", "\\\\")
    cpp_string = f'#pragma once\n\nnamespace web {{\nconstexpr inline const char* html = R"(\n{html_content}\n)";\n}}'

    with open(output_file_path, "w", encoding="utf-8") as file:
        file.write(cpp_string)

    print(f"转换完成，输出文件：{output_file_path}")


html_file_path = "src/WebServer.html"
output_file_path = "src/page.html.hh"
convert_html_to_cpp(html_file_path, output_file_path)

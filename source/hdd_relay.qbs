import qbs
import qbs.FileInfo
import qbs.TextFile

Project {
    references: [
        "stm8_libs/stm8_libs.qbs"
    ]

Product {
    name: "common_options"

    Export {
        Depends { name: "cpp" }

        cpp.defines: [
            "STM8S103",
            "F_CPU=2000000UL",
            "__STDC_LIMIT_MACROS",
        ]

        cpp.commonCompilerFlags: [
            "-e",
            "--eec++",
            "--mfc",
            "--diag_suppress=Pa137,Go004,Go005",
        ]

        cpp.optimization: "small"
    }
}

CppApplication {
    name: "hdd_relay"

    Depends { name: "stm8lib" }
    Depends { name: "common_options" }

    type: ["print_size"]

    cpp.positionIndependentCode: false
    cpp.debugInformation: false
    cpp.generateLinkerMapFile: true

// Broken for Qbs 1.22+: bugreports.qt.io/browse/QBS-1704
    Group {
        name: "Linker script"
        prefix: cpp.toolchainInstallPath + "/../config/"
        fileTags: ["linkerscript"]
        files: ["lnkstm8s103f3.icf"]
    }

    cpp.driverLinkerFlags: [
        "--config_def", "_CSTACK_SIZE=0x80",
        "--config_def", "_HEAP_SIZE=0",
        "--merge_duplicate_sections",
    ]

    cpp.includePaths: [

    ]

    Group {
        name: "Compiled object file"
        fileTagsFilter: "application"
        qbs.install: true
    }

    files: [
        "main.cpp",
        "relay.cpp",
        "relay.h",
        "text_display.cpp",
        "text_display.h",
    ]

    Rule {
        inputs: ["application"]
        outputFileTags: ["print_size"]
        prepare: {
            var reverseIndexOf = function(s, entry, position) {
                while(s[position--] !== entry)
                    ;
                return position;
            }
            var mapPath = FileInfo.joinPaths(product.buildDirectory, product.name + ".map");
            var cmd = new JavaScriptCommand();
            var file = new TextFile(mapPath);
            var content = file.readAll();
            file.close()
            var begin = reverseIndexOf(content, "\n", content.indexOf("bytes of")) + 2;
            var end = content.indexOf("\n", content.lastIndexOf("bytes of")) + 1
            cmd.description = content.slice(begin, end);
            cmd.highlight = "Sections";
            return cmd;
        }
    }

} //CppApplication

} //Project

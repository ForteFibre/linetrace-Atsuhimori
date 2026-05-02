Import("env")

env.Append(CXXFLAGS=["-Wno-register"])

env.Tool("compilation_db")
env.CompilationDatabase("compile_commands.json")
env.Replace(COMPILATIONDB_INCLUDE_TOOLCHAIN=True)

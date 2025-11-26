# Zweek Lisp (Prototype)

**A Neuro-Symbolic Compiler for Local AI.**
*Optimizing C++ generation for small-parameter models through a token-efficient Intermediate Representation.*

---

## ⚡ The Problem
Small local LLMs (0.5B - 3B parameters) struggle to generate valid C++ code. They hallucinate syntax, forget semicolons, and waste context window tokens on verbose boilerplate.

## 🛠 The Solution
**Zweek** is a strict, Lisp-based dialect designed specifically for AI generation.
1.  **AI generates Zweek Lisp:** Constrained by GBNF grammars to guarantee 100% valid syntax.
2.  **Zweek Transpiler:** Parses the IR into an AST and transpiles it into optimized, memory-safe C++17.
3.  **Native Speed:** Compiles to raw binary using MSVC/GCC.

## 💻 Current Status
**v0.1.0 - Transpiler Prototype**
The core C++ engine is functional. It currently handles:
- ✅ **S-Expression Parsing:** Tokenizing and AST generation.
- ✅ **Type Inference:** Maps `(let x 10)` to `auto x = 10;`.
- ✅ **Function Definitions:** Transpiles `(def ...)` to typed C++ functions.
- ✅ **I/O Streaming:** Flattens `(print ...)` into `std::cout` chains.
- ✅ **Math:** Recursive expression parsing (`(+ a b)`).
- ✅ **Auto-Headers:** Automatically injects `#include <iostream>`, `<vector>`, etc.

## 📝 Example

**Input (`hello.zw`):**
```lisp
(def add ((a :int) (b :int)) :int
  (+ a b))

(def main () :int
  (let x 10)
  (let y 20)
  (print "Hello from Zweek!")
  (print "10 + 20 =" (add x y))
  0)
```

**Transpiled Output (Generated C++):**
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <memory>

int add(int a, int b) {
    return (a + b);
}

int main() {
    auto x = 10;
    auto y = 20;
    std::cout << "Hello from Zweek!" << std::endl;
    std::cout << "10 + 20 =" << add(x, y) << std::endl;
    return 0;
}
```

## 🏗 Architecture
1.  **Inference:** `llama.cpp` + Qwen-0.5B + Custom GBNF Grammar.
2.  **IR:** Zweek Lisp (S-Expressions).
3.  **Compiler:** Custom C++ AST Parser & Transpiler.
4.  **Backend:** `cl.exe` (Windows) / `g++` (Linux).

---
*Built for the edge. Powered by C++.*
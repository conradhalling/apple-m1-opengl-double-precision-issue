# Minimal repro - Double-precision values in fragment shader issue on Apple M1 GPU

Using a double-precision value in fragment shader leads to software-rendering fallback on Apple M1 GPU. This project is a minimal repro case.

## Build instructions

- `git clone https://github.com/jlevallois/apple-m1-opengl-double-precision-issue.git`
- `cd apple-m1-opengl-double-precision-issue`
- `mkdir build`
- `cd build`
- `cmake ..`
- `cmake --build .`
- `./main`

## Expected output

### On a macbook pro Intel

No log in the console

### On a macbook pro Apple M1

The console displays the following:

```txt
FALLBACK (log once): Fallback to SW vertex processing because buildPipelineState failed
FALLBACK (log once): Fallback to SW fragment processing because buildPipelineState failed
FALLBACK (log once): Fallback to SW vertex processing, m_disable_code: 1000
FALLBACK (log once): Fallback to SW fragment processing, m_disable_code: 1000000
FALLBACK (log once): Fallback to SW vertex processing in drawCore, m_disable_code: 1000
FALLBACK (log once): Fallback to SW fragment processing in drawCore, m_disable_code: 1000000
```

leading to very bad performance (software mode is very slow).

## Workaround

Change the following line in the fragment shader: `double coucou = 0.5;` to `float coucou = 0.5f;` seems to resolve the issue but you will lose some precision (in this example we don't care, but it can be an issue).

:octocat:
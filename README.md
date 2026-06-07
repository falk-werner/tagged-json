# Tagged JSON

## Desgin Goals

- Easy to parse
- Human readable
- Schema support
- Comments
- Ordered Maps
- Explicit Number Types
- Multi line strings
- Binary data
- Trailing data, e.g. for digital signatures

## Rules

| Type | Example | Description |
| ---- | ------- | ----------- |
| Comment | `# Comment` | Comment |
| None | `none` | None |
| Bool | `true` `false` | Boolean value |
| Unsigned int | `123` | Unsigned int |
| Signed int | `-123` `+123` | Signed int |
| Float | `123.0` | Float |
| String | `"Hello"` | String |
| Binary Data | `b64:` | Binary data, base64 encoded |
| Sequence | `[1,2,3]` | Sequence |
| Ordered Map | `("key": "value")` | Ordered map. |
| Unordered Map | `{"key": "value"}` | Unordered map. |
| Tag | `@tag` | Type tag |
| End | `;` | End of Input |

### Comment

```
# This is a comment
```

### None

```
none
```

### Bool

```
true
false
```

### Numeric Types

#### unsigned int

```
123
```

#### signed int

```
-123
+123
```

#### float

```
f1.0
```

### Binary Data

```
hex:1234
b64:abcd
```

### Sequences

```
[1, 2, 3]
```

### Ordered Maps

```
(
    "key": value
)
```

### Unordered Maps

```
{
    "key": "value"
}
```

### tags

```
@u8 32
@str "foo"
@method_call ("method": "add", "params": [1, 2], "id": 42)
```

### trailer

```
;any trailing data
```
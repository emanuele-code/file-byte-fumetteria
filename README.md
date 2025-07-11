# file-byte-fumetteria
C++ file management project focusing on processing in `ios::binary` mode.
The file is processed assuming that the byte blocks are `80 bytes` in size, as given by the `Fumetto` struct. This allows the file cursor to be moved block by block, making it more efficient to find and remove comic books from the file.

<br>

## Struct description

The structure has been optimized to use a sufficien number of bytes to prevent the compiler from rounding the size to a multiple of 4 (memory alignment) to allow hardware efficiency as excpected from modern x86 architectures.   
This avoids the need to use `#pragmas` which would lead to inefficiencies and little portability of the software.

```cpp
typedef struct {
  int  numero;
  char titolo[26];
  char note[50];
} Fumetto;
```

<br>

## Inserisci Fumetto function description
Function used to add the rappresentation of a `Fumetto` in the file.  

<ul>
  <li>
    <a href="https://devdocs.io/cpp/io/basic_istream/ignore" target="_blank"><code>cin.ignore(80, '\n')</code></a>
    <ul>
      <li>Ignores characters up to <code>count</code> or until the <code>delim</code> character is found.</li>
    </ul>
  </li>
  <li>
    <a href="https://devdocs.io/cpp/io/basic_istream/getline" target="_blank"><code>cin.getline(fumetto.titolo, 26);</code></a>
    <ul>
      <li>Reads characters into a buffer up to <code>count</code> or until the <code>delim</code> character is encountered.</li>
    </ul>
  </li>
  <li>
    <a href="https://devdocs.io/cpp/io/basic_ostream/write" target="_blank"><code>file.write(reinterpret_cast&lt;char*&gt;(&fumetto), sizeof(Fumetto));</code></a>
    <ul>
      <li><code>write()</code> is used when working with files opened using <code>ios::binary</code>, and it writes data to a file.</li>
      <li><code>reinterpret_cast&lt;char*&gt;(&fumetto)</code> is one of four C++ cast types. This one specifically converts the object into a byte stream.</li>
    </ul>
  </li>
</ul>




## Cerca fumetto function description
Function used to search a `fumetto` in the file.

<ul>
  <li>
    <a href="https://devdocs.io/cpp/io/basic_istream/ignore" target="_blank"><code>streampos fileSize = file.tellg()</code></a>
    <ul>
      <li>return the current position in byte from the start of the file.</li>
      <li><code>streampos</code> cannot be divided by the struct dimensione in order to get how many <code>fumetti</code> I have, therefore, <code>streamoff fileSizeInt = static_cast<streamoff>(fileSize)</code> is necessary to cast to a type that is usually a type definied as <code>long long</code> or <code>int_64t</code> </li> 
      <li><code>off_t</code> is an <code>unsigned integer</code> which rapresents a shift in a file.</li>
    </ul>
  </li>
  <li>
    <a href="" target="_blank"><code>truncate(NOME_FILE, sizeNuova)</code></a>
    <ul>
      <li>it gets the path to the file and truncate to the specified size, deleting whatever is at the end.</li>
    </ul>
  </li>
</ul>
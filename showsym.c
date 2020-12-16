// Template for parsing an ELF file to print its symbol table
// UPDATED: Tue Dec  8 03:27:18 PM CST 2020 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

// The below macros help to prevent errors when doing pointer
// arithmetic. Adding an offset to a pointer is the most common
// operation here while the other macros may not be needed.

#define PTR_PLUS_BYTES(ptr,off) ((void *) (((size_t) (ptr)) + ((size_t) (off))))
// macro to add a byte offset to a pointer, arguments are a pointer
// and a # of bytes (usually size_t)

#define PTR_MINUS_BYTES(ptr,off) ((void *) (((size_t) (ptr)) - ((size_t) (off))))
// macro to subtract a byte offset from a pointer, arguments are a pointer
// and a # of bytes (usually size_t)

#define PTR_MINUS_PTR(ptr,ptq) ((long) (((size_t) (ptr)) - ((size_t) (ptq))))
// macro to subtract one pointer from another

int DEBUG = 0;                  
// Controls whether to print debug messages
// Can be used in conditionals like if(DEBUG){ ... }
// and running 'showsym -d x.o' will set DEBUG=1

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s [-d] <file>\n",argv[0]);
    return 0;
  }

  char *objfile_name = argv[1];

  // check for debug mode
  if(argc >=3){
    if(strcmp("-d",argv[1])==0){
      DEBUG = 1;
      objfile_name = argv[2];
    }
    else{
      printf("incorrect usage\n");
      return 1;
    }
  }
                        
  // Open file descriptor and set up memory map for objfile_name
  int fd = open(objfile_name, O_RDONLY);    // File descriptor of the input file
  struct stat sizer;                        // Sizer used to get size of file (in bytes)
  fstat(fd, &sizer);
  int size = sizer.st_size;                 // Size of the file to mmap

  char *file_top = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);   // Map the file in memory and assign to a pointer, call it the file top.

  // CREATE A POINTER to the intial bytes of the file which are an ELF64_Ehdr struct
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *) file_top;


  // CHECK e_ident field's bytes 0 to 3 for the sequence {0x7f,'E','L','F'}.
  // Exit the program with code 1 if the bytes do not match
  int is_elf = 
    ehdr->e_ident[0] == 0x7F &&
    ehdr->e_ident[1] == 'E'  &&
    ehdr->e_ident[2] == 'L'  &&
    ehdr->e_ident[3] == 'F';
  if (!is_elf) {
    printf("ERROR: Magic bytes wrong, this is not an ELF file\n");
    return 1;
  }

  // PROVIDED: check for a 64-bit file
  if(ehdr->e_ident[EI_CLASS] != ELFCLASS64){
    printf("Not a 64-bit file ELF file\n");
    return 1;
  }

  // PROVIDED: check for x86-64 architecture
  if(ehdr->e_machine != EM_X86_64){
    printf("Not an x86-64 file\n");
    return 1;
  }

  // DETERMINE THE OFFSET of the Section Header Array (e_shoff), the
  // number of sections (e_shnum), and the index of the Section Header
  // String table (e_shstrndx). These fields are from the ELF File
  // Header.

  Elf64_Off shoff = ehdr -> e_shoff;        // section header array offset
  Elf64_Half shnum = ehdr -> e_shnum;       // number of sections
  Elf64_Half shstrndx = ehdr -> e_shstrndx; // index of the section header string table

  // Set up a pointer to the array of section headers. Use the section
  // header string table index to find its byte position in the file
  // and set up a pointer to it.
  Elf64_Shdr *sec_hdrs = (Elf64_Shdr *) PTR_PLUS_BYTES(file_top, shoff); // Points to the beginning of the sh array.
  char *sec_names = (char *) PTR_PLUS_BYTES(file_top, sec_hdrs[shstrndx].sh_offset);     // Points to the beginning of the section header string table.
  

  // Search the Section Header Array for the secion with name .symtab
  // (symbol table) and .strtab (string table).  Note their positions
  // in the file (sh_offset field).  Also note the size in bytes
  // (sh_size) and and the size of each entry (sh_entsize) for .symtab
  // so its number of entries can be computed.
  long symtab_off = -1, symtab_size = -1, symtab_entsize = -1;     // Declare variables for several tracking purposes
  long strtab_off = -1, strtab_size = -1;
  
  for(int i=0; i<shnum; i++){
    if (strcmp(sec_names + sec_hdrs[i].sh_name, ".symtab") == 0) {  // if the current section is the symtab section...

      symtab_off = sec_hdrs[i].sh_offset;                          // Get the values we need
      symtab_size = sec_hdrs[i].sh_size;
      symtab_entsize = sec_hdrs[i].sh_entsize;

    } else if (strcmp(sec_names + sec_hdrs[i].sh_name, ".strtab") == 0) { // if the current section is the strtab section...

      strtab_off = sec_hdrs[i].sh_offset;                                // Get the values we need
      strtab_size = sec_hdrs[i].sh_size;

    }
  }

  if(symtab_off == -1 || symtab_size == -1 || symtab_entsize == -1){  // If the offset or the size or the entry size weren't mutated, we didn't find anything
    printf("ERROR: Couldn't find symbol table\n");
    return 1;
  }

  if(strtab_off == -1 || strtab_size == -1){                          // Same for the strtab
    printf("ERROR: Couldn't find string table\n");
    return 1;
  }

  // PRINT byte information about where the symbol table was found and
  // its sizes. The number of entries in the symbol table can be
  // determined by dividing its total size in bytes by the size of
  // each entry.
  printf("Symbol Table\n");
  printf("- %ld bytes offset from start of file\n", symtab_off);
  printf("- %ld bytes total size\n", symtab_size);
  printf("- %ld bytes per entry\n", symtab_entsize);
  long symtab_num;  // Contains the number of symbols

  if (symtab_entsize != 0) {symtab_num = symtab_size / symtab_entsize;} // Calculate symtab_num
  else {symtab_num = 0;}
  
  printf("- %ld entries\n", symtab_num);


  // Set up pointers to the Symbol Table and associated String Table
  // using offsets found earlier.
  Elf64_Sym * symtab = (Elf64_Sym *) PTR_PLUS_BYTES(file_top, symtab_off);    // point to beginning of symbol table
  char * strtab = (char *) PTR_PLUS_BYTES(file_top, strtab_off);                       // point to beginning of string table


  // Print column IDs for info on each symbol
  printf("[%3s]  %8s %4s %s\n",
         "idx","TYPE","SIZE","NAME");

  // Iterate over the symbol table entries
  for(int i=0; i<symtab_num; i++){
    // Determine size of symbol and name. Use <NONE> name has zero
    // length.
    unsigned long symsize = symtab[i].st_size;
    char * symname = "";
    
    if (symtab[i].st_name == 0) { // If the name of the symbol is at the beginning of the strtable then we know it has no name.
      symname = "<NONE>";
    } else {
      symname = strtab + symtab[i].st_name;     // Get the name out of the strtable 
    }
    

    // Determine type of symbol. See assignment specification for
    // fields, macros, and definitions related to this.
    char * symtype = "";
    unsigned char typec = ELF64_ST_TYPE(symtab[i].st_info);

    if (typec == STT_NOTYPE) {
      symtype = "NOTYPE";
    } else if (typec == STT_OBJECT) {
      symtype = "OBJECT";
    } else if (typec == STT_FUNC) {
      symtype = "FUNC";
    } else if (typec == STT_FILE) {
      symtype = "FILE";
    } else if (typec == STT_SECTION) {
      symtype = "SECTION";
    }

    // Print symbol information
    printf("[%3d]: %8s %4lu %s\n", i, symtype, symsize, symname);

  }


  // Unmap file from memory and close associated file descriptor 
  munmap(file_top, size);
  close(fd);
  return 0;
}

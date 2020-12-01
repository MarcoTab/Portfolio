.text
.global thermo_update

## Called to update the thermometer display.  Makes use of
## set_temp_from_ports() and set_display_from_temp() to access
## temperature sensor then set the display. Checks these functions and
## if they indicate an error, makes not changes to the display.
## Otherwise modifies THERMO_DISPLAY_PORT to set the display.

## No Arguments
thermo_update:
    pushq   $0                                      # Push an empty temp_t struct onto the stack

    movq    %rsp, %rdi                              # Move the struct at the top of the stack into the first argument
    call set_temp_from_ports                        # Call set_temp_from_ports(&rdi)
    
    cmpq    $0, %rax                                # Compare the return value to 0
    jne     .ERROR_TU                               # If not equal, jump to error and return 1

    movq    (%rdi), %rdi                            # Copy the packed temp_t struct into the first argument
    leaq    THERMO_DISPLAY_PORT(%rip), %rsi         # Copy a pointer to the display temp into the second argument
    call set_display_from_temp                      # Call set_display_from_temp(rdi, &THERMO_DISPLAY_PORT)

    cmpq    $0, %rax                                # Compare the return value to 0
    jne     .ERROR_TU                               # If not equal, jum to error and return 1
	
    popq    %rax                                    # Pop the packed temp_t struct into %rax, which is going to get overwritten anyways
    movq    $0, %rax                                # Change return value to 0 because success
    ret                                             # Return 0

.ERROR_TU:          ## One of the functions returned a value != 0
    popq    %rax                                    # Pop the temp_t struct into %rax, which will get overwritten anyways
    movq    $1, %rax                                # Change return value to 1 because failure
    ret                                             # Return 1



.text
.global  set_temp_from_ports

## Uses the two global variables (ports) THERMO_SENSOR_PORT and
## THERMO_STATUS_PORT to set the temp structure. If THERMO_SENSOR_PORT
## is above its maximum trusted value, associated with +50.0 deg C,
## does not alter temp and returns 1.  Otherwise, sets fields of temp
## based on converting sensor value to degrees and checking whether
## Celsius or Fahrenheit display is in effect. Returns 0 on successful
## set. This function DOES NOT modify any global variables but may
## access global variables.


## Function takes a single argument: temp_t *temp
##                                           %rdi

set_temp_from_ports:

    ## Clear registers for use

    movq    $0, %rsi
    movq    $0, %rcx
    movq    $0, %r8
    movq    $0, %r9

    movw    THERMO_SENSOR_PORT(%rip), %si   # Copy temp reading to reg si  (16-bit word)
    movb    THERMO_STATUS_PORT(%rip), %cl   # Copy units (C or F) to reg cl  (8-bit word)
    
    cmpw     $64000, %si                    # Compare reading to highest possible temp 
    ja       .ERROR_STFP                    # If higher, invalid
    cmpw     $0, %si                        # Compare reading to lowest possible temp
    jb       .ERROR_STFP                    # If lower, invalid

    movl    %esi, %r8d                      # Will use this to divide by 64
    movl    %esi, %r9d                      # Will use this to find remainder of dividing by 64
    
    sar     $6, %r8d                        # Shift right 6 times (same as dividing by 64)
    andl    $0b111111, %r9d                 # %r9w now contains the remainder of dividing by 64      
    
    cmpw    $32, %r9w                       # If remainder is greater than or equal to 32...
    jge     .ROUND_UP                       # Round up!
                                            # ELSE
    subw    $500, %r8w                      # Take away 500

    jmp     .CHECK_UNITS                    ## We don't want to accidentally subtract 499 after already subtracting 500

.ROUND_UP:

    subw    $499, %r8w                      # Take away 499 to effectively round up

.CHECK_UNITS:                               ## We'll be doing any conversions if needed. This label is mostly just to skip the ROUND_UP label
    
    andb    $0b1, %cl                       # Only care if very first byte is 1
    cmpb    $1, %cl                         # Compare the now "anded" value to 1
    je      .CONVERT_TO_F                   # If it is one, we need to do a conversion
                                            # ELSE
    movb    $0, 2(%rdi)                     # Place a 0 where it belongs (temp->is_fahrenheit)
    jmp     .RET_STFP                       # We're done here


.CONVERT_TO_F:
    
    movb    $1, 2(%rdi)                     # Place a 1 where it belongs (temp->is_fahrenheit)
    movw    %r8w, %ax                       # Move to %ax for division
    imulw   $9, %ax                         # Start conversion by multiplying by 9

    cwtl                                    # sign extend ax to long word
    cltq                                    # sign extend eax to quad word
    cqto                                    # sign extend ax to edx

    movw    $5, %r8w                        # Move 5 to a register to perform division
    idivw   %r8w                            # Divide by 5
    
    addw    $320, %ax                       # Finally add 320 to finish conversion

    movw    %ax, %r8w                       # Move back to %r8w to "return"

    
.RET_STFP:
    movw    %r8w, (%rdi)                    # Before returning, assign the temp value to where it belongs (temp->tenths_degrees)
    movq    $0, %rax                        # Set return value to 0 because success
    ret                                     # Return 0


.ERROR_STFP:
    movq    $1, %rax                        # Set return value to 1 because failure
    ret                                     # Return 1






.data           ## Define global variables to use for masking and ORing

negmask:        # Contains a mask to indicate negation. If shifted 28 marks C, 29 marks F
        .int    0b1

maskarray:      # An array containing the necessary masks to set the display #
        .int    126         # 0
        .int    12          # 1
        .int    55          # 2
        .int    31          # 3
        .int    77          # 4
        .int    91          # 5
        .int    123         # 6
        .int    14          # 7
        .int    127         # 8
        .int    95          # 9


.text
.global  set_display_from_temp

## Alters the bits of integer pointed to by display to reflect the
## temperature in struct arg temp.  If temp has a temperature value
## that is below minimum or above maximum temperature allowable or if
## an improper indication of celsius/fahrenheit is given, does nothing
## and returns 1. Otherwise, calculates each digit of the temperature
## and changes bits at display to show the temperature according to
## the pattern for each digit.  This function DOES NOT modify any
## global variables but may access global variables.

## Arguments are:       temp_t temp, int *display                 
##                             %rdi      %rsi
set_display_from_temp:
    
                            ## Clearing out and setting registers for use
    movq    $0, %r8                                     # Helper register. Several uses.
    movq    $0, %r9                                     # Helper register. Keeps the value of the temp we're lookng at
    movq    $1000, %r11                                 # To use to divide various times 
    movq    $10, %r10                                   # Same as r11
    movq    $0, %rcx                                    # Helper register. Will store shifted masks from time to time


    movq    %rdi, %r8                                   # Copy the struct to a separate register to mask and shift and do comparisons
    movq    %rdi, %r9                                   # Copy the struct to a separate register to mask and shift and do comparisons
                               
    sar     $16, %r8                                    # Shift right 16 bits to access the .is_fahrenheit field.
    
    andb    $-1, %r8b                                   # Look at the last byte, AND it with 0b1111 1111 = -1
    andq    $0b00000000000000001111111111111111, %r9    # AND the value in %r9w with 0b1111 1111 1111 1111
    
    cmpb    $1, %r8b                                    # Check the unit value against 1
    jg      .ERROR_SDFT                                 # Invalid unit value
    je      .F_TEMPCHECK                                # Go to the temp check for fahrenheit, since it's not going to be less than 0...

    cmpb    $0, %r8b                                    # Check the unit value against 0
    jl      .ERROR_SDFT                                 # Invalid unit value

                            ## Temp is in C, check that it is between -50 and 50

    cmpw    $500, %r9w                                  # Check the temp value against 500
    jg      .ERROR_SDFT                                 # Too large, invalid

    cmpw    $-500, %r9w                                 # Check the temp value against -500
    jl      .ERROR_SDFT                                 # Too negative, invalid

    jmp     .ACTUALLY_START

.F_TEMPCHECK:               ## Temp check for Fahrenheit is different than for Celsius

    cmpw    $1220, %r9w                                 # Check the temp value against 1220
    jg      .ERROR_SDFT                                 # Too large, invalid

    cmpw    $-580, %r9w                                 # Check the temp value against -580
    jl      .ERROR_SDFT                                 # Too negative, invalid


.ACTUALLY_START:            ## PHEW, we've cleared all the tests, now it's time to do the setting!
    
    leaq    maskarray(%rip), %rdi                       # Make rdi a pointer to the beginning of the maskarray, which contains the masks for values

    movl    $0, (%rsi)                                  # Set display to 0b00...00 

    cmpw    $0, %r9w                                    # First I want to know if the temp is negative
    jge     .START_TEMP_EXTRACTION_LOOP                 # If not, skip the next stuff that is only important for negative temperatures

    neg     %r9w                                        # Negate negative number for ease of use.

    leaq    negmask(%rip), %rdi                         # We're not using rdi anymore, might as well recycle.
    movq    (%rdi), %rcx                                # Move the negative mask into a register
    orl     %ecx, (%rsi)                                # OR onto display...
    sall    $7, (%rsi)                                  # Shift display left by 7

    leaq    maskarray(%rip), %rdi                       # Make rdi a pointer to the beginning of the maskarray, which contains the masks for values 


.START_TEMP_EXTRACTION_LOOP: ## This loop extracts the numbers contained in temp (%r9w) and OR's each value onto the display

    cmpq    %r11, %r9                                   # Compare r9 to the current value in r11
    jl      .LOOP_RESET                                 # Only run the body of the loop if r9 >= r11

    movq    %r9, %rax                                   # Move the value in r9 to rax

    cqto                                                # Sign extend rax to rdx
    idivq   %r11                                        # Divide rax by r11

    cqto                                                # Sign extend rax to rdx
    idivq   %r10                                        # Since r10 contains $10, rdx now has the value to print to console

    movq    (%rdi, %rdx, 4), %rcx                       # Move the correct bitmask to rcx
    orl     %ecx, (%rsi)                                # OR the value in rcx with the display.
    sall    $7, (%rsi)                                  # Shift display left by 7

.LOOP_RESET:               ## This subset of the loop is the decremental part of the loop, divides r11 by 10 and then checks it against 10
    movq    %r11, %rax                                  # Move the value in r11 to rax
    cqto                                                # Sign extend rax to rdx
    idivq   %r10                                        # Divide rax by $10
    movq    %rax, %r11                                  # Move the divided value into r11

    cmpq    $10, %r11                                   # Compare r11 to $10
    jne     .START_TEMP_EXTRACTION_LOOP                 # If not equal to $10, then redo the loop wiht the new r11 value 

.END_TEMP_EXTRACTION_LOOP: ## Two last extractions: the ones and tenths place. We can execute these unconditionally, no matter the value

    movq    %r9, %rax                                   # Move the value in r9 to rax
    cqto                                                # Sign extend rax to rdx
    idivq   %r10                                        # Divide by 10
    cqto                                                # Sign extend rax to rdx
    idivq   %r10                                        # Here, if the number was larger than 10, we are holding the value for the ones place. If not, we hold 0!

    movq    (%rdi, %rdx, 4), %rcx                       # Move the correct bitmask to rcx
    orl     %ecx, (%rsi)                                # OR the value in rcx with the display
    sall    $7, (%rsi)                                  # Shift display left by 7

    movq    %r9, %rax                                   # Move the value in r9 to rax
    cqto                                                # Sign extend rax to rdx
    idivq   %r10                                        # rdx contains what goes in the tenths place
    movq    (%rdi, %rdx, 4), %rcx                       # Move the correct bitmask to rcx
    orl     %ecx, (%rsi)                                # OR the value in RCX with the display

.UNIT_DISPLAYING:          ## Onto showing our units! In like a completely non creepy way

    leaq    negmask(%rip), %rdi                         # Again, not using number or negative masks, so let's use the same register
    movq    (%rdi), %rcx                                # to use for ORing
    cmpb    $0, %r8b                                    # Check against 0
    je      .C                                          # I have a really neat trick up my sleeve :)
    
    salq    $1, %rcx                                    # Left shift 1 time for F

.C:                        ## Label to do neat thing with shifting
    salq    $28, %rcx                                   # Shift left 28 times for C, but if it was F, then it will be shifted a totla of 29 times. HA!   
    orl     %ecx, (%rsi)                                # OR it 

.RET_SDFT:
    movq    $0, %rax                                    # Change return value to 0 because success
    ret                                                 # Return 0

.ERROR_SDFT:               ## One of the inputs was incorrect
    movq    $1, %rax                                    # Change return value to 1 because failure
    ret                                                 # Return 1




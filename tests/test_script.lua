-- Define a global variable
global_var = 10

-- Function to print a message
local function printMessage(message)
    print("Message: " .. message)
end

-- Function to add two numbers
local function add(a, b)
    return a + b
end

-- Function to calculate the factorial of a number
local function factorial(n)
    if n == 0 then
        return 1
    else
        return n * factorial(n - 1)
    end
end

-- Main function
local function main()
    print("Starting the script...")

    -- Print a message
    printMessage("Hello, LuaWatch!")

    -- Add two numbers
    local sum = add(3, 7)
    print("Sum: " .. sum)

    -- Calculate factorial
    local num = 5
    local result = factorial(num)
    print("Factorial of " .. num .. " is " .. result)

    -- Modify the global variable
    global_var = global_var + 1
    print("Global variable: " .. global_var)
end

-- Call the main function
main()

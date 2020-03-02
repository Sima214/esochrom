-- Settings.
local PATTERN_FUNCTION = "^([%w:]+) Get([%w]+)%([%w%s]*%)"
local PATTERN_CASE = "^%s*case ([%w_]+)"
local PATTERN_PATTERN = "^(%s*)/%* (%w+) %*/$"
local INSET_DIRS = {"_TOP", "_LEFT", "_BOTTOM", "_RIGHT"}

-- Utils.
local print_ini_file = print

-- Check arguments.
local args = table.pack(...)
local source_filename = args[1]

if not source_filename then
    print("Please provide the path to layout_constants.cc as the first argument.")
end

-- Parse source file.
print("Loading `"..source_filename.."`...")

local cases = {}

local source = assert(io.open(source_filename, 'r'))
local last_function = nil
local last_function_is_inset = false
local last_case = nil
for l in source:lines() do
    -- Add a new section.
    if l:find(PATTERN_FUNCTION) then
        ret_type, new_function = l:match(PATTERN_FUNCTION)
        if ret_type == "gfx::Insets" then
            last_function_is_inset = true
        elseif ret_type == "int" then
            last_function_is_inset = false
        else
            print(string.format("Unknown return type `%s` for function `%s`!", ret_type, new_function))
            os.exit(1)
        end
        if last_function then
            print_ini_file()
        end
        print_ini_file(string.format("[%s]", new_function))
        cases[new_function] = {inset=last_function_is_inset}
        last_function = new_function
    elseif l:find(PATTERN_CASE) then
        case_label = l:match(PATTERN_CASE)
        if last_function_is_inset then
            for _, d in ipairs(INSET_DIRS) do
                local obj = {name = case_label..d, value = 8}
                table.insert(cases[last_function], obj)
                print_ini_file(string.format("%s=%d", obj.name, obj.value))
            end
        else
            local obj = {name = case_label, value = 24}
            table.insert(cases[last_function], obj)
            print_ini_file(string.format("%s=%d", obj.name, obj.value))
        end
    end
end
source:close()

-- Convert pattern file to source.
local function script_path()
    local str = debug.getinfo(2, "S").source:sub(2)
    return str:match("(.*/)") or "./"
end

local pattern_filename = script_path().."layout_constants_pattern.cc"
local pf = assert(io.open(pattern_filename, 'r'))
local sf = assert(io.open(source_filename, 'w'))

local function write_labels(padding, case_labels)
    for i, v in ipairs(case_labels) do
        sf:write(padding.."\""..v.name.."\",")
        sf:write("\n")
    end
end

local function write_cases(padding, case_labels)
    if not case_labels.inset then
        for i, v in ipairs(case_labels) do
            sf:write(padding.."case "..v.name..":")
            sf:write("\n")
            sf:write(padding.."  return loaded_layout[offset + "..(i-1).."];")
            sf:write("\n")
        end
    else
        for i=1, #case_labels, 4 do
            a = case_labels[i]
            b = case_labels[i + 1]
            c = case_labels[i + 2]
            d = case_labels[i + 3]
            local name = a.name:sub(1, -5)
            sf:write(padding.."case "..name..":")
            sf:write("\n")
            sf:write(padding.."  return gfx::Insets(loaded_layout[offset + "..(i-1).."], loaded_layout[offset + "..(i).."], loaded_layout[offset + "..(i+1).."], loaded_layout[offset + "..(i+2).."]);")
            sf:write("\n")
        end
    end
end

for l in pf:lines() do
    local padding, id = l:match(PATTERN_PATTERN)
    if id then
        if id == "CocoaLayoutLabels" then
            write_labels(padding, cases.CocoaLayoutConstant)
        elseif id == "LayoutLabels" then
            write_labels(padding, cases.LayoutConstant)
        elseif id == "LayoutInsetsLabels" then
            write_labels(padding, cases.LayoutInsets)
        elseif id == "CocoaLayoutConstant" then
            write_cases(padding, cases.CocoaLayoutConstant)
        elseif id == "LayoutConstant" then
            write_cases(padding, cases.LayoutConstant)
        elseif id == "LayoutInsets" then
            write_cases(padding, cases.LayoutInsets)
        else
            print("Cannot do anything for `"..id.."`!")
            os.exit(1)
        end
    else
        sf:write(l)
        sf:write("\n")
    end
end
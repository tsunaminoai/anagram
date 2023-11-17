const std = @import("std");

const Alpha = u26;

/// The main function of the program. It handles command line arguments, memory allocation, and calls other functions to load words from a file, check words, and print the results.
pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    var alloc = gpa.allocator();

    const args = try std.process.argsAlloc(alloc);
    defer std.process.argsFree(alloc, args);

    var wordFile: []u8 = undefined;
    var query: []u8 = undefined;

    for (args[1..], 1..) |a, i| {
        if (std.mem.eql(u8, "-d", a)) {
            wordFile = try alloc.alloc(u8, args[i + 1].len);
            std.mem.copy(u8, wordFile, args[i + 1]);
        }
        if (std.mem.eql(u8, "-q", a)) {
            query = try alloc.alloc(u8, args[i + 1].len);
            std.mem.copy(u8, query, args[i + 1]);
        }
    }
    defer alloc.free(wordFile);
    defer alloc.free(query);

    // todo: make the dictionary comptime and store it in the bin
    var dictionary = try loadWords(wordFile, alloc);
    defer {
        for (dictionary.items) |*w|
            w.deinit(alloc);
        dictionary.deinit();
    }
    var result = try checkWords(query, dictionary, alloc);
    defer result.deinit();

    // std.debug.print("Found {d} results\n", .{result.items.len});
    try print(result);
}

/// Prints the words in the given dictionary to the standard output.
fn print(dict: std.ArrayList(codeWord)) !void {
    const stdoutF = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdoutF);
    const stdout = bw.writer();

    for (dict.items) |word| {
        try stdout.print("{s}\n", .{word.word});
        // try stdout.print("{s}\t{b}\n", .{ word.word, word.code });
    }
    try bw.flush();
}

/// A struct representing a word and its corresponding code.
const codeWord = struct {
    word: []u8,
    code: Alpha,

    pub fn init(string: []const u8, alloc: std.mem.Allocator) !codeWord {
        var cw: codeWord = .{
            .word = try alloc.alloc(u8, string.len),
            .code = wordToCode(string),
        };
        @memcpy(cw.word, string);
        return cw;
    }

    pub fn deinit(self: @This(), alloc: std.mem.Allocator) void {
        alloc.free(self.word);
    }
};

/// Converts a word to a code.
fn wordToCode(word: []const u8) Alpha {
    var code: Alpha = 0;
    for (word) |c| {
        code |= @as(Alpha, @intCast(0x1)) << @as(u5, @intCast(c - 'A'));
    }
    return code;
}

/// Loads words from a file and returns an ArrayList of codeWords.
fn loadWords(filename: []const u8, alloc: std.mem.Allocator) !std.ArrayList(codeWord) {
    var words = std.ArrayList(codeWord).init(alloc);
    // std.debug.print("Loading file: {s}\n", .{filename});
    var file = try std.fs.cwd().openFile(filename, .{});
    defer file.close();

    var contents = try file.readToEndAlloc(alloc, 1024 * 1024 * 10);
    defer alloc.free(contents);

    var lines = std.mem.split(u8, contents, "\n");
    while (lines.next()) |line| {
        if (line.len <= 0)
            break;
        var cw = try codeWord.init(line, alloc);
        try words.append(cw);
    }
    // std.debug.print("Loaded {d} words\n", .{words.items.len});

    return words;
}

/// A comparison function for sorting.
fn sort(ctx: []u8, lhs: u8, rhs: u8) bool {
    _ = ctx;
    return lhs < rhs;
}

/// Checks if the needle word is present in the haystack of words.
fn checkWords(needle: []const u8, haystack: std.ArrayList(codeWord), alloc: std.mem.Allocator) !std.ArrayList(codeWord) {
    var results = std.ArrayList(codeWord).init(alloc);
    const code = wordToCode(needle);
    // std.debug.print("needle: {s}, len: {d}\n", .{ needle, needle.len });
    for (haystack.items) |word| {
        if (code & word.code == word.code and word.word.len <= needle.len) {
            var buffer = try alloc.alloc(u8, 1);
            defer alloc.free(buffer);

            var append = true;
            for (word.word) |l| {
                buffer[0] = l;
                var letterCountReference = std.mem.count(u8, word.word, buffer);
                var letterCountQuery = std.mem.count(u8, needle, buffer);
                if (letterCountQuery < letterCountReference)
                    append = false;
            }
            if (append)
                try results.append(word);
        }
    }

    return results;
}

test "codeword" {
    var alloc = std.testing.allocator;

    const word = "WORD";
    var cw = try codeWord.init(word, alloc);
    defer cw.deinit(alloc);
    try std.testing.expectEqual(cw.code, 0b00010000100100000000001000);
}

test "load" {
    var alloc = std.testing.allocator;
    var filename = "words.txt";
    var wordList = try loadWords(filename, alloc);
    defer {
        for (wordList.items) |*w|
            w.deinit(alloc);
        wordList.deinit();
    }
    try std.testing.expectEqual(wordList.items.len, 4);

    const check = "ELPAP";
    var res = try checkWords(check, wordList, alloc);
    defer res.deinit();

    try std.testing.expectEqual(res.items.len, 1);
    try std.testing.expect(std.mem.eql(u8, "APPLE", res.items[0].word));
}

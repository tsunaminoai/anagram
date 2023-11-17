const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "anagram",
        .root_source_file = .{ .path = "src/main.zig" },
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the zig solution using binary encoding");
    run_step.dependOn(&run_cmd.step);

    const cexe = b.addExecutable(.{
        .name = "anagram-c",
        .target = target,
        .optimize = optimize,
    });

    cexe.addCSourceFiles(&[_][]const u8{ "c/anagram.c", "c/results.c", "c/trie.c" }, &[_][]const u8{"-O3"});
    cexe.addIncludePath(.{ .path = "./c/" });
    b.installArtifact(cexe);
    const run_C_step = b.step("runc", "Run the c solution using trees (command line args do differ)");
    const run_C_cmd = b.addRunArtifact(exe);
    run_C_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_C_cmd.addArgs(args);
    }
    run_C_step.dependOn(&run_C_cmd.step);

    const unit_tests = b.addTest(.{
        .root_source_file = .{ .path = "src/main.zig" },
        .target = target,
        .optimize = optimize,
    });

    const run_unit_tests = b.addRunArtifact(unit_tests);

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_unit_tests.step);
}

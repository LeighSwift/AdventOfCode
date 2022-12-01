const string InputFile = @"input.txt";

string[] inputLines = File.ReadAllLines(InputFile);

int elfCalories = 0;
int[] topThree = new int[3] {0,0,0};
for (int lineIdx = 0; lineIdx < inputLines.Length; lineIdx++)
{
    string line = inputLines[lineIdx];
    if (string.IsNullOrWhiteSpace(line) || (lineIdx+1) == inputLines.Length)
    {
        topThree[0] = Math.Max(topThree[0], elfCalories);
        Array.Sort(topThree);
        elfCalories = 0;
        continue;
    }
    elfCalories += int.Parse(line);
}

// Answer to part one.
Console.WriteLine($"Highest:          {topThree[2]}");
// Answer to part two.
Console.WriteLine($"Sum of top three: {topThree.Sum()}");

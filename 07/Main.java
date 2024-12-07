import java.util.*;

public class Main
{
  public static void main(String[] args)
  {
    long total           = 0;
    long totalWithConcat = 0;
    Scanner in = new Scanner(System.in);
    while (in.hasNextLine()) {
      Scanner scanLine = new Scanner(in.nextLine());
      scanLine.useDelimiter(":? ");
      long target = scanLine.nextLong();
      ArrayList<Long> values = new ArrayList<>();
      while (scanLine.hasNextLong()) {
        values.add(scanLine.nextLong());
      }

      if (hasSolution(values, target)) {
        total += target;
      }
      if (hasSolutionWithConcat(values, target)) {
        totalWithConcat += target;
      }
    }

    System.out.printf("%d %d\n", total, totalWithConcat);
  }

  public static boolean hasSolution(ArrayList<Long> values, Long target)
  {
    return hasSolutionGo(values, target, 0, 0);
  }

  public static boolean hasSolutionGo(ArrayList<Long> values, long target, long acc, int i)
  {
    if (i == values.size()) {
      return acc == target;
    } else {
      return hasSolutionGo(values, target, acc * values.get(i), i + 1) ||
             hasSolutionGo(values, target, acc + values.get(i), i + 1);
    }
  }

  public static boolean hasSolutionWithConcat(ArrayList<Long> values, Long target)
  {
    return hasSolutionWithConcatGo(values, target, 0, 0);
  }

  public static boolean hasSolutionWithConcatGo(ArrayList<Long> values, long target, long acc, int i)
  {
    if (i == values.size()) {
      return acc == target;
    } else {
      long concatResult = Long.valueOf(Long.toString(acc).concat(Long.toString(values.get(i))));
      return hasSolutionWithConcatGo(values, target, concatResult, i + 1) ||
             hasSolutionWithConcatGo(values, target, acc * values.get(i), i + 1) ||
             hasSolutionWithConcatGo(values, target, acc + values.get(i), i + 1);
    }
  }
}

module Main ( main ) where

import Data.List (group, lookup, sort)
import Data.Maybe (fromMaybe)
import System.Environment (getArgs)
import System.Exit
import System.IO

main :: IO ()
main = do
  args <- getArgs
  if (1 /= length args)
   then do
     hPutStrLn stderr "usage: ./day01 <file>"
     exitFailure
   else do
     pairs <- readPairs $ args !! 0
     print $ solvePartOne pairs
     print $ solvePartTwo pairs

readPairs :: String -> IO [(Int, Int)]
readPairs filename = do
  contents <- readFile filename
  return $ map readPair $ lines contents
  where readPair :: String -> (Int, Int)
        readPair s = let [a, b] = map read $ words s
                     in (a, b)

solvePartOne :: [(Int, Int)] -> Int
solvePartOne pairs = let (listA, listB) = unzip pairs
                         sortedDiffs = zipWith (\x y -> abs $ x - y) (sort listA) (sort listB)
                     in sum sortedDiffs

solvePartTwo :: [(Int, Int)] -> Int
solvePartTwo pairs = let (listA, listB) = unzip pairs
                         counts = map groupToCount $ group $ sort listB
                                   where groupToCount :: [Int] -> (Int, Int)
                                         groupToCount (x:xs) = (x, length xs + 1)
                         similarities = map (\x -> x * (fromMaybe 0 $ x `lookup` counts)) listA
                     in sum similarities

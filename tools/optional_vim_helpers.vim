" vim: set et ts=2 sw=0 sts=0 :

" A 'single-shot' fold function for my personal C++ coding style.
"
" The rest of this file (as of this writing) consists entirely of
" helper functions and other implementation details.
function! g:MyCppFold()
  set foldmethod=manual
  normal zRzE

  call s:Fold.HeaderComments()
  call s:Fold.MajorBlocks()
  call s:Fold.CommentBlocks.function()
endfunction

let s:slashx2 = '\/\/'  "  '//' properly escaped, but actually discernible.

" The main dictionary for all things comment-folding.
let s:Fold = {}

" 'Special' two-line 'header' comments, with level determined by indentation.
"
" They are of the following general form (all '//'s start in column 1):
"
" // Top-Level Header
" //------------------
"
" // There should be empty lines before and after every header comment.
"
" //   A Sub-Heading, Indented By Two More Spaces
" //  --------------------------------------------
"
" // The first line ("title line") cannot be empty. It must be preceded by an
" // odd number of spaces.
" //
" // The dashes in the second line are required to be preceded by an even number
" // of spaces, EXACTLY one less than the number of leading spaces on the
" // title line.
" //
" // It can consist ONLY of the column-1 double slash, leading spaces, and at
" // least three dashes.
"
" //   A Sibbling Sub-Heading
" //  ------------------------
"
" // The fold created by a header comment ends when it reaches another header
" // comment of equal or lesser indentation, when it reaches a special
" // end-fold comment of equal or lesser indentation, or when it reaches the
" // end of the file.
" //
" // End-fold comments are exactly like header comments, except that their
" // lines are reversed, and the header-title line is allowed to be blank.
" //
" // There is NO enforcement making sure that an end-fold comment's title line
" // matches the title line of the header-comment for which it is closing the
" // fold.
"
" //---
" //
"
" // Header comments (& end-fold comments) should be placed so as not to
" // define folds that split the end of any fold from its beginning. In fact,
" // their anticipated use sees them as 'global scope' folds, never being
" // contained by any other folds except other header-based folds.
"
function! s:Fold.HeaderComments()
  let l:level=5
  while l:level >= 0
    silent execute 'global'
      \ . '/\v^'
      \ .   s:slashx2 . '(%(  ){' . l:level . '})' . ' \S.*'   . '\n'
      \ .   s:slashx2 . '\1'                       . '-{3,}'   . '$'
      \ . '/'
      \ . ':normal zf'
      \ . '/\v'
      \ .   '%(^'                                              . '\n'
      \ .     s:slashx2 . '(%(  ){,'. l:level . '})'. ' \S.*'  . '\n'
      \ .     s:slashx2 . '\1'                      . '-{3,}'  . '$'
      \ .   '|'
      \ .     s:slashx2 . '(%(  ){,'. l:level . '})'. '-{3,}'  . '\n'
      \ .     s:slashx2 . '\zs%(\2 \S.*)?\s*'                  . '$'
      \ .   '|'
      \ .     '%$'
      \ .   ')'
      \ . '/0' . "\<CR>" . 'zo'

    let l:level-=1
  endwhile
endfunction

function! s:Fold.MajorBlocks()
  silent global /^\s*{\s*$/
    \ :normal g_zf%zo

  " I arbitrarily decided I still want to treat namespaces as folding major
  " blocks, even though I like having the opening brace on the same line as
  " the rest of the declaration.
  silent execute 'global'
    \ . '/\v^\s*'
    \ .   '%(<inline>\_s+)?'
    \ .   'namespace .*'
    \ .   '\zs\{'
    \ . '\s*$/'
    \ . ':normal g_zf%zo'

  " Kaleidoscope (keyboard firmware) Macro block
  silent global
    \ /\v[A-Z_]+\s*\(\s*$/
    \ :normal g_zf%zo
endfunction

let s:Fold.CommentBlocks = {}

" For unknown and totally unknowable reasons, the global command ignores the
" `/zs` directive in its pattern argument. It is still provided below for
" expository reasons.
"
" the entry “beginning” is what we actually want, but it doesn't work.
" Provided for expository reasons.
let s:Fold.CommentBlocks.patterns = #{
\ beginning:
  \   '\v'
  \ . '%('
  \ .   '%^'
  \ . '|'
  \ .   '%(^\s*' . s:slashx2 . '.*)@<!' . '\n'
  \ . ')'
  \ . '\zs'
  \ . '(\s*)' . s:slashx2 . '.*'        . '\n'
  \ . '\1'    . s:slashx2 . '.*'        . '\n'
  \ . '\1'    . s:slashx2
  \ ,
\ beginning_a_first_line_of_file:
  \   '\v'
  \ . '%^'
  \ ,
\ beginning_a_literally_anywhere_else:
  \   '\v'
  \ . '%(^\s*' . s:slashx2 . '.*)@<!' . '\n'
  \ ,
\ beginning_b:
  \   '\zs'
  \ . '(\s*)' . s:slashx2 . '.*'      . '\n'
  \ . '\1'    . s:slashx2 . '.*'      . '\n'
  \ . '\1'    . s:slashx2
  \ ,
\ ending:
  \   '\v'                            . '\n'
  \ . '%(\s*' . s:slashx2 . ')@!'
\}

" Has to be multiple commands because :global is stupid. It's very important
" to note the differences in the ':normal' part--that's literally the whole
" point!
"
" This could be factored out better, but I kinda *want* to highlight how
" inadequate the `global` command is.
let s:Fold.CommentBlocks.commands = #{
\ first_line_of_file:
  \   'global'
  \ . '/'
  \ .   s:Fold.CommentBlocks.patterns.beginning_a_first_line_of_file
  \ .   s:Fold.CommentBlocks.patterns.beginning_b
  \ . '/'
  \ . ':normal zf'
  \ . '/'
  \ .   s:Fold.CommentBlocks.patterns.ending
  \ . '/0' . "\<cr>" . 'zo'
  \ ,
\ literally_anywhere_else:
  \   'global'
  \ . '/'
  \ .   s:Fold.CommentBlocks.patterns.beginning_a_literally_anywhere_else
  \ .   s:Fold.CommentBlocks.patterns.beginning_b
  \ . '/'
  \ . ':normal jzf'
  \ . '/'
  \ .   s:Fold.CommentBlocks.patterns.ending
  \ . '/0' . "\<cr>" . 'zo'
  \ ,
\}

function! s:Fold.CommentBlocks.function()
  for key in keys(s:Fold.CommentBlocks.commands)
    silent execute s:Fold.CommentBlocks.commands[key]
  endfor
endfunction

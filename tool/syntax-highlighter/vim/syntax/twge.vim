syntax keyword twgeKeyword block actions checks triggers true false def const
syntax match twgeString /"[^"]*"/
syntax match twgeComment /\/\/.*$/
syntax match twgeMetadata /\v__\w+__/

highlight twgeKeyword ctermfg=Blue guifg=Blue
highlight twgeString ctermfg=Green guifg=Green
highlight twgeComment ctermfg=Grey guifg=Grey
highlight twgeMetadata ctermfg=Magenta guifg=Magenta
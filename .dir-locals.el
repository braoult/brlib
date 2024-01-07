;; emacs per-directory settings
((nil . ((compile-command . (concat "make -C "
                                    (vc-root-dir)
                                    " -k -j2")))))

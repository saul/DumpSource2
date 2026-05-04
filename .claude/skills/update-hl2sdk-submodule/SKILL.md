---
name: update-hl2sdk-submodule
description: Update an hl2sdk-* vendor submodule (cs2, dota, deadlock) to the latest commit on its tracked branch. Use when the user asks to "update the SDK", "bump hl2sdk", "update the deadlock/cs2/dota submodule", or pull latest SDK changes.
---

# Updating hl2sdk-* submodules

The `vendor/hl2sdk-{cs2,dota,deadlock}` submodules all point at the same upstream repo (`alliedmodders/hl2sdk`) but each tracks a different branch, declared in `.gitmodules`:

| Submodule path             | Branch     |
| -------------------------- | ---------- |
| `vendor/hl2sdk-cs2`        | `cs2`      |
| `vendor/hl2sdk-dota`       | `dota`     |
| `vendor/hl2sdk-deadlock`   | `deadlock` |

## The gotcha

Each submodule's local clone has a **restricted fetch refspec** — it only fetches the one branch it was originally cloned with. So `git fetch origin <branch>` succeeds but does *not* update `refs/remotes/origin/<branch>`, which makes `git checkout <branch>` and `git checkout origin/<branch>` both fail.

You must fetch the branch explicitly into the remote-tracking ref.

## Procedure

Replace `<sub>` with the submodule name (`cs2`, `dota`, or `deadlock`) and `<branch>` with the matching branch from the table above.

```bash
git -C vendor/hl2sdk-<sub> fetch origin <branch>:refs/remotes/origin/<branch>
git -C vendor/hl2sdk-<sub> checkout -B <branch> origin/<branch>
git -C vendor/hl2sdk-<sub> log -1 --oneline
```

Then in the parent repo, stage the new pointer:

```bash
git add vendor/hl2sdk-<sub>
```

Do **not** commit unless the user asks. Report the old → new commit hashes and the new commit subject.

## Alternative: `git submodule update --remote`

`git submodule update --remote vendor/hl2sdk-<sub>` respects the branch in `.gitmodules` and works without the explicit refspec dance, but leaves the submodule in detached HEAD. Prefer the explicit procedure above so the submodule ends up on a named local branch matching upstream.

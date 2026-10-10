# Reusable download standard for DeadneM mod projects

**For every future mod build and every ChatGPT conversation:**

1. Build with Windows GitHub Actions and confirm build/package/upload success.
2. Confirm ZIP integrity and place the DLL, ASI, INI and README directly at ZIP root, with no surrounding directory.
3. Retain a **versioned** permanent backup ZIP in repository `downloads/`.
4. Publish a **rolling unvalidated test prerelease** with tag `test-build` and fixed ZIP asset `Mod_LATEST_TEST.zip`.
5. Primary link: `https://github.com/OWNER/REPO/releases/download/test-build/Mod_LATEST_TEST.zip`.
6. Secondary link: versioned ZIP at `https://raw.githubusercontent.com/OWNER/REPO/main/downloads/Mod_Vx.y.zip`.
7. Files attached directly in ChatGPT are helpful but **not the sole delivery method** because client downloads can fail.
8. Never send signed `productionresults*.blob.core.windows.net` or `release-assets.githubusercontent.com` URLs: they expire.
9. **Do not upgrade an official validated Release** without explicit user validation. The rolling test asset is always a prerelease, marked UNVALIDATED.

The rolling release workflow implementation is available in [build-asi-overlay.yml](../.github/workflows/build-asi-overlay.yml).
Reuse its last step for another GitHub repository and adapt the fixed tag asset name and versioned source ZIP.
Ensure workflow permissions include `contents: write`, and verify a run actually published the asset.

### Darksiders Genesis links

- Rolling test: https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases/download/test-build/DarksidersGenesis_LATEST_TEST.zip
- Backup V0.64: https://raw.githubusercontent.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/main/downloads/DarksidersGenesis_V0.64.zip
- Official releases: https://github.com/DeadneM/Darksiders-Genesis-Enhanced-PC-Mod/releases

A stable GitHub Releases URL can internally redirect through time-limited CDN URLs, but the **starting URL remains stable** and generates a fresh redirect every time.
Antivirus blocks are independent of link lifetime; avoid telling users to turn off security protections.

### Other chats

Use this document as the distribution standard when working on any other DeadneM mod.
The document is a reusable instruction, not a claim that unrelated repositories were already modified.

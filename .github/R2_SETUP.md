# Cloudflare R2 setup for shell-1-256M.v86b

The release workflow uploads `shell-1-256M.v86b` to the `epitech` R2 bucket and serves it publicly at:

```
https://cdn.cazal.eu/shell-1-256M.v86b
```

## One-time Cloudflare configuration

R2 does not support object-level ACLs via the S3 API. Public access is configured at the bucket level.

### 1. Connect custom domain `cdn.cazal.eu`

In **Cloudflare dashboard → R2 → epitech → Settings → Custom Domains**:

1. Click **Add domain** and enter `cdn.cazal.eu`.
2. Cloudflare auto-creates the DNS record (zone `cazal.eu` must already be on Cloudflare).
3. Wait for the domain status to show **Active**.

Do not use the r2.dev development URL for production traffic.

### 2. Configure CORS (required for browser fetch)

The `.v86b` file is loaded cross-origin by the shell-1 web app. Add a CORS policy on the `epitech` bucket:

```json
[
  {
    "AllowedOrigins": ["https://your-shell-1-domain.com"],
    "AllowedMethods": ["GET", "HEAD"],
    "AllowedHeaders": ["*"],
    "ExposeHeaders": ["ETag", "Content-Length"],
    "MaxAgeSeconds": 3600
  }
]
```

Configure via **R2 → epitech → Settings → CORS policy**, or Wrangler CLI.

## GitHub Actions secrets

Add these in **Settings → Secrets and variables → Actions**:

| Secret | Value |
|--------|-------|
| `R2_ACCOUNT_ID` | Cloudflare account ID |
| `R2_S3_ENDPOINT` | `https://<account-id>.r2.cloudflarestorage.com` |
| `R2_ACCESS_KEY_ID` | From Cloudflare R2 API token |
| `R2_SECRET_ACCESS_KEY` | From Cloudflare R2 API token |
| `R2_PUBLIC_BASE_URL` | `https://cdn.cazal.eu` |

Create the R2 API token in **Cloudflare dashboard → R2 → Manage R2 API Tokens**.

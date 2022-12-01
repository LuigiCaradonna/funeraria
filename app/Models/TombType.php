<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\Tomb;

class TombType extends Model
{
    use HasFactory;
    
    /**
     * The attributes that are mass assignable.
     *
     * @var array<string, int>
     */
    protected $fillable = [
        'name',
        'thickness',
    ];
    
    /**
     * Relation with the tombs.
     */
    public function tombs()
    {
        return $this->hasMany(Tomb::class, 'id');
    }
}
